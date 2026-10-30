#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <vector>

namespace sl
{
    /** Axis-aligned rectangle: top-left corner plus size. */
    struct Rect2D
    {
        float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;
    };

    /** Circle: centre plus radius. */
    struct Circle2D
    {
        float x = 0.0f, y = 0.0f, radius = 0.0f;
    };

    /** A 2D point or vector. */
    struct Vec2D
    {
        float x = 0.0f, y = 0.0f;
    };

    /** Result of a collision test: whether the shapes overlap and how to push the first one out of the second. */
    struct Collision2D
    {
        bool hit = false;
        /** Unit direction to move the first shape to separate it from the second. */
        Vec2D normal;
        /** Distance to move along the normal to separate the shapes. */
        float depth = 0.0f;
    };

    /** Result of a ray cast. */
    struct RayHit2D
    {
        bool hit = false;
        /** Distance along the ray (0 = origin, 1 = end when the direction is the full ray vector). */
        float t = 0.0f;
        Vec2D point;
        /** Surface normal at the hit point, facing the ray origin. */
        Vec2D normal;
    };

    inline bool point_in_rect(float px, float py, const Rect2D &r)
    {
        return px >= r.x && px < r.x + r.width && py >= r.y && py < r.y + r.height;
    }

    inline bool point_in_circle(float px, float py, const Circle2D &c)
    {
        const float dx = px - c.x, dy = py - c.y;
        return dx * dx + dy * dy <= c.radius * c.radius;
    }

    inline bool rects_overlap(const Rect2D &a, const Rect2D &b)
    {
        return a.x < b.x + b.width && b.x < a.x + a.width && a.y < b.y + b.height && b.y < a.y + a.height;
    }

    inline bool circles_overlap(const Circle2D &a, const Circle2D &b)
    {
        const float dx = b.x - a.x, dy = b.y - a.y, r = a.radius + b.radius;
        return dx * dx + dy * dy <= r * r;
    }

    inline bool circle_rect_overlap(const Circle2D &c, const Rect2D &r)
    {
        const float nx = std::clamp(c.x, r.x, r.x + r.width);
        const float ny = std::clamp(c.y, r.y, r.y + r.height);
        const float dx = c.x - nx, dy = c.y - ny;
        return dx * dx + dy * dy <= c.radius * c.radius;
    }

    /** Collide two rectangles and report the smallest push that separates a from b. */
    inline Collision2D collide_rects(const Rect2D &a, const Rect2D &b)
    {
        Collision2D result;
        if (!rects_overlap(a, b))
            return result;
        const float push_right = b.x + b.width - a.x;
        const float push_left = a.x + a.width - b.x;
        const float push_down = b.y + b.height - a.y;
        const float push_up = a.y + a.height - b.y;
        const float x_depth = std::min(push_right, push_left);
        const float y_depth = std::min(push_down, push_up);
        result.hit = true;
        if (x_depth <= y_depth)
        {
            result.depth = x_depth;
            result.normal = {push_right < push_left ? 1.0f : -1.0f, 0.0f};
        }
        else
        {
            result.depth = y_depth;
            result.normal = {0.0f, push_down < push_up ? 1.0f : -1.0f};
        }
        return result;
    }

    /** Collide two circles and report the push that separates a from b. */
    inline Collision2D collide_circles(const Circle2D &a, const Circle2D &b)
    {
        Collision2D result;
        const float dx = a.x - b.x, dy = a.y - b.y;
        const float distance = std::sqrt(dx * dx + dy * dy);
        const float reach = a.radius + b.radius;
        if (distance >= reach)
            return result;
        result.hit = true;
        result.depth = reach - distance;
        result.normal = distance > 1e-6f ? Vec2D{dx / distance, dy / distance} : Vec2D{1.0f, 0.0f};
        return result;
    }

    /** Collide a circle with a rectangle and report the push that separates the circle from the rectangle. */
    inline Collision2D collide_circle_rect(const Circle2D &c, const Rect2D &r)
    {
        Collision2D result;
        const float nx = std::clamp(c.x, r.x, r.x + r.width);
        const float ny = std::clamp(c.y, r.y, r.y + r.height);
        float dx = c.x - nx, dy = c.y - ny;
        const float distance_squared = dx * dx + dy * dy;
        if (distance_squared > c.radius * c.radius)
            return result;
        result.hit = true;
        if (distance_squared > 1e-12f)
        {
            const float distance = std::sqrt(distance_squared);
            result.normal = {dx / distance, dy / distance};
            result.depth = c.radius - distance;
            return result;
        }
        // Centre is inside the rectangle: leave through the nearest edge.
        const float left = c.x - r.x, right = r.x + r.width - c.x;
        const float top = c.y - r.y, bottom = r.y + r.height - c.y;
        const float nearest = std::min({left, right, top, bottom});
        if (nearest == left)
            result.normal = {-1.0f, 0.0f};
        else if (nearest == right)
            result.normal = {1.0f, 0.0f};
        else if (nearest == top)
            result.normal = {0.0f, -1.0f};
        else
            result.normal = {0.0f, 1.0f};
        result.depth = nearest + c.radius;
        return result;
    }

    /**
     * Test two convex polygons with the separating axis theorem. Vertices must be in order (either winding).
     * The normal pushes polygon a out of polygon b.
     */
    inline Collision2D collide_polygons(const std::vector<Vec2D> &a, const std::vector<Vec2D> &b)
    {
        Collision2D result;
        if (a.size() < 3 || b.size() < 3)
            return result;
        float best_depth = std::numeric_limits<float>::max();
        Vec2D best_axis;
        for (int pass = 0; pass < 2; ++pass)
        {
            const std::vector<Vec2D> &poly = pass == 0 ? a : b;
            for (std::size_t i = 0; i < poly.size(); ++i)
            {
                const Vec2D p = poly[i], q = poly[(i + 1) % poly.size()];
                Vec2D axis{q.y - p.y, p.x - q.x};
                const float length = std::sqrt(axis.x * axis.x + axis.y * axis.y);
                if (length < 1e-9f)
                    continue;
                axis = {axis.x / length, axis.y / length};
                float min_a = std::numeric_limits<float>::max(), max_a = -min_a;
                float min_b = min_a, max_b = -min_a;
                for (const Vec2D &v : a)
                {
                    const float d = v.x * axis.x + v.y * axis.y;
                    min_a = std::min(min_a, d);
                    max_a = std::max(max_a, d);
                }
                for (const Vec2D &v : b)
                {
                    const float d = v.x * axis.x + v.y * axis.y;
                    min_b = std::min(min_b, d);
                    max_b = std::max(max_b, d);
                }
                if (max_a <= min_b || max_b <= min_a)
                    return result;
                const float overlap_forward = max_b - min_a;
                const float overlap_back = max_a - min_b;
                const float depth = std::min(overlap_forward, overlap_back);
                if (depth < best_depth)
                {
                    best_depth = depth;
                    best_axis = overlap_forward < overlap_back ? axis : Vec2D{-axis.x, -axis.y};
                }
            }
        }
        result.hit = true;
        result.depth = best_depth;
        result.normal = best_axis;
        return result;
    }

    /** Cast a ray from (ox, oy) along (dx, dy) against a rectangle. t is a fraction of the (dx, dy) vector in [0, 1]. */
    inline RayHit2D raycast_rect(float ox, float oy, float dx, float dy, const Rect2D &r)
    {
        RayHit2D result;
        float t_min = 0.0f, t_max = 1.0f;
        Vec2D normal;
        const float origin[2] = {ox, oy}, dir[2] = {dx, dy};
        const float lo[2] = {r.x, r.y}, hi[2] = {r.x + r.width, r.y + r.height};
        for (int axis = 0; axis < 2; ++axis)
        {
            if (std::abs(dir[axis]) < 1e-9f)
            {
                if (origin[axis] < lo[axis] || origin[axis] > hi[axis])
                    return result;
                continue;
            }
            float t1 = (lo[axis] - origin[axis]) / dir[axis];
            float t2 = (hi[axis] - origin[axis]) / dir[axis];
            float sign = -1.0f;
            if (t1 > t2)
            {
                std::swap(t1, t2);
                sign = 1.0f;
            }
            if (t1 > t_min)
            {
                t_min = t1;
                normal = axis == 0 ? Vec2D{sign, 0.0f} : Vec2D{0.0f, sign};
            }
            t_max = std::min(t_max, t2);
            if (t_min > t_max)
                return result;
        }
        result.hit = true;
        result.t = t_min;
        result.point = {ox + dx * t_min, oy + dy * t_min};
        result.normal = normal;
        return result;
    }

    /** Cast a ray from (ox, oy) along (dx, dy) against a circle. t is a fraction of the (dx, dy) vector in [0, 1]. */
    inline RayHit2D raycast_circle(float ox, float oy, float dx, float dy, const Circle2D &c)
    {
        RayHit2D result;
        const float fx = ox - c.x, fy = oy - c.y;
        const float a = dx * dx + dy * dy;
        if (a < 1e-12f)
            return result;
        const float b = 2.0f * (fx * dx + fy * dy);
        const float k = fx * fx + fy * fy - c.radius * c.radius;
        const float discriminant = b * b - 4.0f * a * k;
        if (discriminant < 0.0f)
            return result;
        const float root = std::sqrt(discriminant);
        float t = (-b - root) / (2.0f * a);
        if (t < 0.0f)
            t = k <= 0.0f ? 0.0f : (-b + root) / (2.0f * a);
        if (t < 0.0f || t > 1.0f)
            return result;
        result.hit = true;
        result.t = t;
        result.point = {ox + dx * t, oy + dy * t};
        const float nx = result.point.x - c.x, ny = result.point.y - c.y;
        const float length = std::sqrt(nx * nx + ny * ny);
        result.normal = length > 1e-9f ? Vec2D{nx / length, ny / length} : Vec2D{-dx, -dy};
        return result;
    }

    /** Uniform-grid broad phase: store ids with bounding rectangles, then query a region for candidates. */
    class SpatialHash2D
    {
    public:
        explicit SpatialHash2D(float cell_size = 64.0f) : cell_size_(cell_size > 0.0f ? cell_size : 64.0f) {}

        /** Remove everything. */
        void clear() { cells_.clear(); }

        /** Add an item. The same id may be inserted several times with different bounds. */
        void insert(int id, const Rect2D &bounds)
        {
            for_each_cell(bounds, [&](std::int64_t key) { cells_[key].push_back(id); });
        }

        /** Return the ids whose cells overlap the region, without duplicates. Callers should run an exact test on them. */
        std::vector<int> query(const Rect2D &region) const
        {
            std::vector<int> found;
            for_each_cell(region, [&](std::int64_t key) {
                const auto cell = cells_.find(key);
                if (cell != cells_.end())
                    found.insert(found.end(), cell->second.begin(), cell->second.end());
            });
            std::sort(found.begin(), found.end());
            found.erase(std::unique(found.begin(), found.end()), found.end());
            return found;
        }

    private:
        template <typename Fn>
        void for_each_cell(const Rect2D &bounds, Fn fn) const
        {
            const int x0 = static_cast<int>(std::floor(bounds.x / cell_size_));
            const int y0 = static_cast<int>(std::floor(bounds.y / cell_size_));
            const int x1 = static_cast<int>(std::floor((bounds.x + bounds.width) / cell_size_));
            const int y1 = static_cast<int>(std::floor((bounds.y + bounds.height) / cell_size_));
            for (int cy = y0; cy <= y1; ++cy)
                for (int cx = x0; cx <= x1; ++cx)
                    fn((static_cast<std::int64_t>(cx) << 32) ^ static_cast<std::uint32_t>(cy));
        }

        float cell_size_;
        std::unordered_map<std::int64_t, std::vector<int>> cells_;
    };
} // namespace sl
