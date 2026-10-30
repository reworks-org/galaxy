#include "props3d.h"

#include <algorithm>
#include <cmath>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

namespace sl
{
    namespace
    {
        using V = glm::vec3;
        using Ring = std::vector<V>;
        constexpr float pi = 3.14159265358979f;

        const Colour glass_dark{36, 50, 68, 255};
        const Colour glass_lit{255, 222, 138, 255};
        const Colour steel{88, 92, 100, 255};
        const Colour dark_metal{44, 46, 52, 255};

        float hash01(std::uint32_t value)
        {
            value ^= value >> 16;
            value *= 0x7feb352dU;
            value ^= value >> 15;
            value *= 0x846ca68bU;
            value ^= value >> 16;
            return static_cast<float>(value & 0xffffffU) / 16777215.0f;
        }

        Colour shade(Colour colour, float factor)
        {
            const auto scale = [&](std::uint8_t channel)
            { return static_cast<std::uint8_t>(std::clamp(static_cast<float>(channel) * factor, 0.0f, 255.0f)); };
            return {scale(colour.red), scale(colour.green), scale(colour.blue), colour.alpha};
        }

        Colour vary(Colour colour, std::uint32_t seed, float amount)
        {
            return shade(colour, 1.0f + amount * (2.0f * hash01(seed * 977u + 13u) - 1.0f));
        }

        Vertex3D make_vertex(const V &position, const V &normal, Colour colour)
        {
            Vertex3D vertex;
            vertex.position[0] = position.x;
            vertex.position[1] = position.y;
            vertex.position[2] = position.z;
            vertex.normal[0] = normal.x;
            vertex.normal[1] = normal.y;
            vertex.normal[2] = normal.z;
            vertex.colour[0] = colour.red / 255.0f;
            vertex.colour[1] = colour.green / 255.0f;
            vertex.colour[2] = colour.blue / 255.0f;
            vertex.colour[3] = colour.alpha / 255.0f;
            return vertex;
        }

        void add_flat_triangle(MeshData &data, const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, Colour colour)
        {
            const glm::vec3 cross = glm::cross(p1 - p0, p2 - p0);
            const glm::vec3 normal = glm::length(cross) > 1e-9f ? glm::normalize(cross) : glm::vec3{0.0f, 1.0f, 0.0f};
            const auto base = static_cast<std::uint32_t>(data.vertices.size());
            data.vertices.push_back(make_vertex(p0, normal, colour));
            data.vertices.push_back(make_vertex(p1, normal, colour));
            data.vertices.push_back(make_vertex(p2, normal, colour));
            data.indices.insert(data.indices.end(), {base, base + 1, base + 2});
        }

        /** A cone with a smooth side and no base, apex up. */
        void add_cone(MeshData &data, float y0, float height, float radius, int sides, Colour colour)
        {
            const float slope = radius / std::max(height, 1e-4f);
            for (int side = 0; side < sides; ++side)
            {
                const float a0 = 2.0f * pi * static_cast<float>(side) / static_cast<float>(sides);
                const float a1 = 2.0f * pi * static_cast<float>(side + 1) / static_cast<float>(sides);
                const float am = 0.5f * (a0 + a1);
                const auto normal = [&](float angle)
                { return glm::normalize(glm::vec3{std::sin(angle), slope, std::cos(angle)}); };
                const auto base = static_cast<std::uint32_t>(data.vertices.size());
                data.vertices.push_back(make_vertex({std::sin(a0) * radius, y0, std::cos(a0) * radius}, normal(a0), colour));
                data.vertices.push_back(make_vertex({std::sin(a1) * radius, y0, std::cos(a1) * radius}, normal(a1), colour));
                data.vertices.push_back(make_vertex({0.0f, y0 + height, 0.0f}, normal(am), colour));
                // Counter-clockwise seen from outside.
                data.indices.insert(data.indices.end(), {base, base + 1, base + 2});
            }
        }

        void add_cylinder(MeshData &data, float y0, float y1, float radius, int sides, Colour colour)
        {
            for (int side = 0; side < sides; ++side)
            {
                const float a0 = 2.0f * pi * static_cast<float>(side) / static_cast<float>(sides);
                const float a1 = 2.0f * pi * static_cast<float>(side + 1) / static_cast<float>(sides);
                const glm::vec3 n0{std::sin(a0), 0.0f, std::cos(a0)};
                const glm::vec3 n1{std::sin(a1), 0.0f, std::cos(a1)};
                const auto base = static_cast<std::uint32_t>(data.vertices.size());
                data.vertices.push_back(make_vertex(n0 * radius + glm::vec3{0.0f, y0, 0.0f}, n0, colour));
                data.vertices.push_back(make_vertex(n1 * radius + glm::vec3{0.0f, y0, 0.0f}, n1, colour));
                data.vertices.push_back(make_vertex(n1 * radius + glm::vec3{0.0f, y1, 0.0f}, n1, colour));
                data.vertices.push_back(make_vertex(n0 * radius + glm::vec3{0.0f, y1, 0.0f}, n0, colour));
                data.indices.insert(data.indices.end(), {base, base + 1, base + 3, base + 1, base + 2, base + 3});
            }
        }

        V centroid(const Ring &ring)
        {
            V sum{0.0f};
            for (const V &point : ring)
                sum += point;
            return sum / static_cast<float>(ring.size());
        }

        /** Emits flat-shaded triangles. Every face is oriented to point away from the `inside` point it is given. */
        struct Builder
        {
            MeshData data;

            void tri(V a, V b, V c, Colour colour, const V &inside)
            {
                V normal = glm::cross(b - a, c - a);
                if (glm::length(normal) < 1e-9f)
                    return;
                normal = glm::normalize(normal);
                if (glm::dot(normal, (a + b + c) / 3.0f - inside) < 0.0f)
                {
                    std::swap(b, c);
                    normal = -normal;
                }
                const auto base = static_cast<std::uint32_t>(data.vertices.size());
                data.vertices.push_back(make_vertex(a, normal, colour));
                data.vertices.push_back(make_vertex(b, normal, colour));
                data.vertices.push_back(make_vertex(c, normal, colour));
                data.indices.insert(data.indices.end(), {base, base + 1, base + 2});
            }

            void quad(const V &a, const V &b, const V &c, const V &d, Colour colour, const V &inside)
            {
                tri(a, b, c, colour, inside);
                tri(a, c, d, colour, inside);
            }

            void loft(const Ring &a, const Ring &b, const std::function<Colour(int)> &colour, const V &inside)
            {
                const int n = static_cast<int>(a.size());
                for (int i = 0; i < n; ++i)
                {
                    const auto j = static_cast<std::size_t>((i + 1) % n);
                    quad(a[static_cast<std::size_t>(i)], a[j], b[j], b[static_cast<std::size_t>(i)], colour(i), inside);
                }
            }

            void loft(const Ring &a, const Ring &b, Colour colour, const V &inside)
            {
                loft(a, b, [&](int) { return colour; }, inside);
            }

            void cap(const Ring &ring, Colour colour, const V &inside)
            {
                const V c = centroid(ring);
                const int n = static_cast<int>(ring.size());
                for (int i = 0; i < n; ++i)
                    tri(c, ring[static_cast<std::size_t>(i)], ring[static_cast<std::size_t>((i + 1) % n)], colour, inside);
            }

            /** A closed solid between two corresponding rings (a convex shape). */
            void prism(const Ring &lo, const Ring &hi, Colour colour)
            {
                const V inside = 0.5f * (centroid(lo) + centroid(hi));
                loft(lo, hi, colour, inside);
                cap(lo, colour, inside);
                cap(hi, colour, inside);
            }

            void append(const MeshData &part, const V &offset, float yaw, float scale)
            {
                const float s = std::sin(yaw);
                const float c = std::cos(yaw);
                const auto base = static_cast<std::uint32_t>(data.vertices.size());
                for (Vertex3D vertex : part.vertices)
                {
                    const float px = vertex.position[0] * scale, py = vertex.position[1] * scale, pz = vertex.position[2] * scale;
                    vertex.position[0] = c * px + s * pz + offset.x;
                    vertex.position[1] = py + offset.y;
                    vertex.position[2] = -s * px + c * pz + offset.z;
                    const float nx = vertex.normal[0], nz = vertex.normal[2];
                    vertex.normal[0] = c * nx + s * nz;
                    vertex.normal[2] = -s * nx + c * nz;
                    data.vertices.push_back(vertex);
                }
                for (const std::uint32_t index : part.indices)
                    data.indices.push_back(index + base);
            }
        };

        Ring rect(float cx, float cz, float hx, float hz, float y)
        {
            return {{cx - hx, y, cz - hz}, {cx + hx, y, cz - hz}, {cx + hx, y, cz + hz}, {cx - hx, y, cz + hz}};
        }

        void perpendicular(const V &axis, V &u, V &v)
        {
            const V reference = std::abs(axis.y) < 0.9f ? V{0.0f, 1.0f, 0.0f} : V{1.0f, 0.0f, 0.0f};
            u = glm::normalize(glm::cross(reference, axis));
            v = glm::cross(axis, u);
        }

        Ring circle(const V &centre, const V &axis, float radius, int sides, float phase = 0.0f)
        {
            V u, v;
            perpendicular(axis, u, v);
            Ring ring;
            for (int i = 0; i < sides; ++i)
            {
                const float angle = phase + 2.0f * pi * static_cast<float>(i) / static_cast<float>(sides);
                ring.push_back(centre + (u * std::cos(angle) + v * std::sin(angle)) * radius);
            }
            return ring;
        }

        /** An elliptical ring in the XY plane at depth z. */
        Ring ring_xy(float z, float cy, float rx, float ry, int sides, float phase = 0.0f)
        {
            Ring ring;
            for (int i = 0; i < sides; ++i)
            {
                const float angle = phase + 2.0f * pi * static_cast<float>(i) / static_cast<float>(sides);
                ring.push_back({rx * std::cos(angle), cy + ry * std::sin(angle), z});
            }
            return ring;
        }

        void box(Builder &b, const V &lo, const V &hi, Colour colour)
        {
            const float cx = 0.5f * (lo.x + hi.x), cz = 0.5f * (lo.z + hi.z);
            b.prism(rect(cx, cz, 0.5f * (hi.x - lo.x), 0.5f * (hi.z - lo.z), lo.y), rect(cx, cz, 0.5f * (hi.x - lo.x), 0.5f * (hi.z - lo.z), hi.y),
                    colour);
        }

        /** A (possibly tapered) cylinder or cone from a to b; a radius of 0 gives an apex. */
        void column(Builder &b, const V &a, const V &c, float r0, float r1, int sides, Colour colour, bool cap_a = true, bool cap_b = true,
                    float phase = 0.0f)
        {
            const V axis = glm::normalize(c - a);
            const Ring ring_a = circle(a, axis, r0, sides, phase);
            const Ring ring_b = circle(c, axis, r1, sides, phase);
            const V inside = 0.5f * (a + c);
            b.loft(ring_a, ring_b, colour, inside);
            if (cap_a && r0 > 1e-5f)
                b.cap(ring_a, colour, inside);
            if (cap_b && r1 > 1e-5f)
                b.cap(ring_b, colour, inside);
        }

        /** Part of a lumpy ellipsoid between two latitudes (radians, -pi/2..pi/2). */
        void ellipsoid(Builder &b, const V &centre, const V &radii, int segments, int rings, float lat0, float lat1, float jitter,
                       std::uint32_t seed, Colour colour, float variation, bool cap_bottom = false)
        {
            std::vector<Ring> grid;
            for (int r = 0; r <= rings; ++r)
            {
                const float lat = lat0 + (lat1 - lat0) * static_cast<float>(r) / static_cast<float>(rings);
                const bool pole = std::abs(std::cos(lat)) < 1e-4f;
                Ring ring;
                for (int s = 0; s < segments; ++s)
                {
                    const float angle = 2.0f * pi * static_cast<float>(s) / static_cast<float>(segments);
                    const float lump = 1.0f + jitter * (2.0f * hash01(seed * 31u + static_cast<std::uint32_t>(r * 131 + (pole ? 0 : s))) - 1.0f);
                    ring.push_back(centre + V{std::cos(lat) * std::sin(angle) * radii.x, std::sin(lat) * radii.y,
                                              std::cos(lat) * std::cos(angle) * radii.z} * lump);
                }
                grid.push_back(ring);
            }
            for (int r = 0; r < rings; ++r)
            {
                for (int s = 0; s < segments; ++s)
                {
                    const auto n = static_cast<std::size_t>((s + 1) % segments);
                    const auto i = static_cast<std::size_t>(s);
                    const Colour facet = shade(colour, 1.0f + variation * (2.0f * hash01(seed * 17u + static_cast<std::uint32_t>(r * 53 + s)) - 1.0f));
                    b.quad(grid[static_cast<std::size_t>(r)][i], grid[static_cast<std::size_t>(r)][n], grid[static_cast<std::size_t>(r + 1)][n],
                           grid[static_cast<std::size_t>(r + 1)][i], facet, centre);
                }
            }
            if (cap_bottom)
                b.cap(grid.front(), shade(colour, 0.7f), centre + V{0.0f, radii.y, 0.0f});
        }

        /** A flat quad facing `normal`, sitting just proud of the surface it is placed on. */
        void panel(Builder &b, const V &centre, const V &normal, float w, float h, Colour colour, const V &up = {0.0f, 1.0f, 0.0f})
        {
            const V right = glm::normalize(glm::cross(up, normal)) * (0.5f * w);
            const V rise = glm::normalize(glm::cross(normal, glm::normalize(glm::cross(up, normal)))) * (0.5f * h);
            const V c = centre + normal * 0.02f;
            b.quad(c - right - rise, c + right - rise, c + right + rise, c - right + rise, colour, centre - normal);
        }

        /** A grid of window panels across one face; `lit` is the fraction that glow. */
        void facade(Builder &b, const V &centre, const V &normal, float length, float y0, int first_floor, int floors, float floor_h,
                    float pitch, float window_w, float window_h, float lit, std::uint32_t seed)
        {
            const V right = glm::cross(V{0.0f, 1.0f, 0.0f}, normal);
            const int columns = std::max(1, static_cast<int>(length / pitch));
            const float step = length / static_cast<float>(columns);
            std::uint32_t id = seed * 7919u + static_cast<std::uint32_t>(std::abs(normal.x * 3.0f + normal.z * 5.0f) + 1.0f) * 131u;
            for (int floor = first_floor; floor < floors; ++floor)
            {
                for (int column = 0; column < columns; ++column)
                {
                    const V position = centre + right * ((static_cast<float>(column) + 0.5f) * step - 0.5f * length) +
                                       V{0.0f, y0 + static_cast<float>(floor) * floor_h + floor_h * 0.55f, 0.0f};
                    const bool on = hash01(++id) < lit;
                    panel(b, V{position.x, position.y, position.z}, normal, window_w, window_h,
                          on ? shade(glass_lit, 0.85f + 0.15f * hash01(id + 99u)) : glass_dark);
                }
            }
        }

        void all_facades(Builder &b, float cx, float cz, float hx, float hz, float y0, int first_floor_front, int floors, float floor_h,
                         float pitch, float window_w, float window_h, float lit, std::uint32_t seed)
        {
            facade(b, {cx, 0.0f, cz + hz}, {0.0f, 0.0f, 1.0f}, 2.0f * hx - 1.0f, y0, first_floor_front, floors, floor_h, pitch, window_w, window_h, lit, seed);
            facade(b, {cx, 0.0f, cz - hz}, {0.0f, 0.0f, -1.0f}, 2.0f * hx - 1.0f, y0, 0, floors, floor_h, pitch, window_w, window_h, lit, seed + 1);
            facade(b, {cx + hx, 0.0f, cz}, {1.0f, 0.0f, 0.0f}, 2.0f * hz - 1.0f, y0, 0, floors, floor_h, pitch, window_w, window_h, lit, seed + 2);
            facade(b, {cx - hx, 0.0f, cz}, {-1.0f, 0.0f, 0.0f}, 2.0f * hz - 1.0f, y0, 0, floors, floor_h, pitch, window_w, window_h, lit, seed + 3);
        }
    } // namespace

    // ----------------------------------------------------------------------------------------------- Buildings

    MeshData make_house_data(float width, float depth, float height, Colour wall, Colour roof, std::uint32_t seed)
    {
        Builder b;
        wall = vary(wall, seed, 0.08f);
        roof = vary(roof, seed + 5u, 0.12f);
        const float hw = 0.5f * width, hd = 0.5f * depth, overhang = 0.3f;
        const float rise = std::min(width, depth) * 0.35f + 0.3f;
        box(b, {-hw, 0.0f, -hd}, {hw, height, hd}, wall);
        const V ridge_a{-hw - overhang, height + rise, 0.0f}, ridge_b{hw + overhang, height + rise, 0.0f};
        const V inside{0.0f, height - 0.5f, 0.0f};
        b.quad({-hw - overhang, height, hd + overhang}, {hw + overhang, height, hd + overhang}, ridge_b, ridge_a, roof, inside);
        b.quad({-hw - overhang, height, -hd - overhang}, {hw + overhang, height, -hd - overhang}, ridge_b, ridge_a, shade(roof, 0.9f), inside);
        for (const float side : {-1.0f, 1.0f})
            b.tri({side * hw, height, -hd}, {side * hw, height, hd}, {side * hw, height + rise, 0.0f}, wall, inside);
        // Chimney, rising through the back slope.
        box(b, {hw * 0.45f, height + rise * 0.2f, -hd * 0.4f}, {hw * 0.45f + 0.5f, height + rise + 0.7f, -hd * 0.4f + 0.5f}, shade(wall, 0.7f));
        panel(b, {0.0f, 1.0f, hd}, {0.0f, 0.0f, 1.0f}, 0.95f, 2.0f, {92, 58, 36, 255});
        const auto window_colour = [&](std::uint32_t k) { return hash01(seed * 41u + k) < 0.5f ? glass_lit : glass_dark; };
        std::uint32_t k = 0;
        for (const float side : {-1.0f, 1.0f})
        {
            panel(b, {side * hw * 0.55f, height * 0.55f, hd}, {0.0f, 0.0f, 1.0f}, 0.9f, 0.9f, window_colour(++k));
            panel(b, {side * hw, height * 0.55f, 0.0f}, {side, 0.0f, 0.0f}, 0.9f, 0.9f, window_colour(++k));
            panel(b, {side * hw * 0.5f, height * 0.55f, -hd}, {0.0f, 0.0f, -1.0f}, 0.9f, 0.9f, window_colour(++k));
        }
        return b.data;
    }

    MeshData make_office_data(float width, float depth, int floors, Colour wall, std::uint32_t seed)
    {
        Builder b;
        floors = std::max(floors, 1);
        wall = vary(wall, seed, 0.06f);
        constexpr float floor_h = 3.5f;
        const float hx = 0.5f * width, hz = 0.5f * depth, total = static_cast<float>(floors) * floor_h;
        box(b, {-hx, 0.0f, -hz}, {hx, total, hz}, wall);
        all_facades(b, 0.0f, 0.0f, hx, hz, 0.0f, 1, floors, floor_h, 2.6f, 1.7f, 1.9f, 0.4f, seed);
        panel(b, {0.0f, 1.3f, hz}, {0.0f, 0.0f, 1.0f}, std::min(3.4f, width * 0.4f), 2.5f, glass_dark);
        // Parapet and rooftop plant.
        const float edge = 0.25f;
        box(b, {-hx, total, hz - edge}, {hx, total + 0.5f, hz}, shade(wall, 0.85f));
        box(b, {-hx, total, -hz}, {hx, total + 0.5f, -hz + edge}, shade(wall, 0.85f));
        box(b, {-hx, total, -hz + edge}, {-hx + edge, total + 0.5f, hz - edge}, shade(wall, 0.85f));
        box(b, {hx - edge, total, -hz + edge}, {hx, total + 0.5f, hz - edge}, shade(wall, 0.85f));
        box(b, {-hx * 0.5f, total, -hz * 0.4f}, {-hx * 0.5f + 2.2f, total + 1.2f, -hz * 0.4f + 1.6f}, steel);
        box(b, {hx * 0.2f, total, hz * 0.1f}, {hx * 0.2f + 1.6f, total + 2.2f, hz * 0.1f + 1.6f}, shade(wall, 0.75f));
        return b.data;
    }

    MeshData make_skyscraper_data(float width, int floors, Colour glass, std::uint32_t seed)
    {
        Builder b;
        floors = std::max(floors, 3);
        glass = vary(glass, seed, 0.06f);
        constexpr float floor_h = 3.6f;
        const int f1 = std::max(1, static_cast<int>(static_cast<float>(floors) * 0.55f));
        const int f2 = std::max(1, static_cast<int>(static_cast<float>(floors) * 0.3f));
        const int f3 = std::max(1, floors - f1 - f2);
        const int tier_floors[3] = {f1, f2, f3};
        const float scale[3] = {1.0f, 0.78f, 0.55f};
        float y = 0.0f;
        float top_hx = 0.0f, top_hz = 0.0f;
        for (int tier = 0; tier < 3; ++tier)
        {
            const float hx = 0.5f * width * scale[tier], hz = 0.5f * width * 0.8f * scale[tier];
            const float height = static_cast<float>(tier_floors[tier]) * floor_h;
            box(b, {-hx, y, -hz}, {hx, y + height, hz}, glass);
            all_facades(b, 0.0f, 0.0f, hx, hz, y, tier == 0 ? 1 : 0, tier_floors[tier], floor_h, 2.0f, 1.4f, 2.1f, 0.35f, seed + static_cast<std::uint32_t>(tier) * 11u);
            if (tier + 1 < 3)
                box(b, {-hx - 0.2f, y + height, -hz - 0.2f}, {hx + 0.2f, y + height + 0.35f, hz + 0.2f}, shade(glass, 0.7f));
            y += height;
            top_hx = hx;
            top_hz = hz;
        }
        panel(b, {0.0f, 1.6f, 0.5f * width * 0.8f}, {0.0f, 0.0f, 1.0f}, std::min(4.0f, width * 0.35f), 3.0f, glass_dark);
        box(b, {-top_hx * 0.5f, y, -top_hz * 0.5f}, {top_hx * 0.5f, y + 2.0f, top_hz * 0.5f}, steel);
        column(b, {0.0f, y + 2.0f, 0.0f}, {0.0f, y + 2.0f + width * 0.7f, 0.0f}, 0.18f, 0.06f, 6, steel);
        box(b, {-0.12f, y + 2.0f + width * 0.7f, -0.12f}, {0.12f, y + 2.0f + width * 0.7f + 0.3f, 0.12f}, {255, 60, 50, 255});
        return b.data;
    }

    MeshData make_hangar_data(float width, float depth, float height, Colour wall, Colour roof)
    {
        Builder b;
        const float hw = 0.5f * width, hd = 0.5f * depth;
        const float wall_h = height * 0.55f, arch_h = height - wall_h;
        std::vector<V> profile{{-hw, 0.0f, 0.0f}, {hw, 0.0f, 0.0f}, {hw, wall_h, 0.0f}};
        constexpr int arc = 8;
        for (int i = 1; i < arc; ++i)
        {
            const float t = pi * static_cast<float>(i) / static_cast<float>(arc);
            profile.push_back({hw * std::cos(t), wall_h + arch_h * std::sin(t), 0.0f});
        }
        profile.push_back({-hw, wall_h, 0.0f});
        Ring front = profile, back = profile;
        for (V &p : front)
            p.z = hd;
        for (V &p : back)
            p.z = -hd;
        const V inside{0.0f, wall_h * 0.5f, 0.0f};
        const int count = static_cast<int>(profile.size());
        b.loft(front, back, [&](int i)
               { return i == 0 ? shade(wall, 0.6f) : (i == 1 || i == count - 1) ? wall : shade(roof, 0.9f + 0.1f * static_cast<float>(i % 2)); },
               inside);
        b.cap(front, wall, inside);
        b.cap(back, wall, inside);
        panel(b, {0.0f, wall_h * 0.9f, hd}, {0.0f, 0.0f, 1.0f}, width * 0.8f, wall_h * 1.8f, {58, 64, 72, 255});
        panel(b, {0.0f, wall_h * 1.9f + 0.1f, hd}, {0.0f, 0.0f, 1.0f}, width * 0.8f, 0.25f, shade(wall, 0.7f));
        return b.data;
    }

    // ----------------------------------------------------------------------------------------------- Structures

    MeshData make_wall_data(float length, float height, float thickness, Colour colour)
    {
        Builder b;
        const float hl = 0.5f * length, ht = 0.5f * thickness;
        box(b, {-hl, 0.0f, -ht}, {hl, height, ht}, colour);
        box(b, {-hl, height, -ht - 0.05f}, {hl, height + 0.15f, ht + 0.05f}, shade(colour, 1.15f));
        for (const float side : {-1.0f, 1.0f})
        {
            const float inner = side * (hl - 0.2f), outer = side * hl;
            box(b, {std::min(inner, outer), 0.0f, -ht - 0.05f}, {std::max(inner, outer), height + 0.2f, ht + 0.05f}, shade(colour, 0.9f));
        }
        for (int course = 1; course < 4; ++course)
        {
            const float y = height * static_cast<float>(course) / 4.0f;
            box(b, {-hl + 0.2f, y - 0.02f, -ht - 0.02f}, {hl - 0.2f, y + 0.02f, ht + 0.02f}, shade(colour, 0.8f));
        }
        return b.data;
    }

    MeshData make_fence_data(float length, float height, Colour colour)
    {
        Builder b;
        const float hl = 0.5f * length;
        const int bays = 4;
        for (int i = 0; i <= bays; ++i)
        {
            const float x = -hl + length * static_cast<float>(i) / static_cast<float>(bays);
            box(b, {x - 0.06f, 0.0f, -0.06f}, {x + 0.06f, height * 1.05f, 0.06f}, shade(colour, 0.8f));
        }
        for (const float level : {0.3f, 0.75f})
            box(b, {-hl, height * level, 0.06f}, {hl, height * level + 0.08f, 0.1f}, shade(colour, 0.9f));
        const int pickets = std::max(2, static_cast<int>(length / 0.25f));
        for (int i = 0; i < pickets; ++i)
        {
            const float x = -hl + (static_cast<float>(i) + 0.5f) * length / static_cast<float>(pickets);
            const float hw = 0.5f * length / static_cast<float>(pickets) * 0.6f;
            box(b, {x - hw, 0.0f, 0.1f}, {x + hw, height * 0.92f, 0.14f}, colour);
            b.tri({x - hw, height * 0.92f, 0.12f}, {x + hw, height * 0.92f, 0.12f}, {x, height, 0.12f}, colour, {x, height * 0.5f, -1.0f});
            b.tri({x - hw, height * 0.92f, 0.12f}, {x + hw, height * 0.92f, 0.12f}, {x, height, 0.12f}, colour, {x, height * 0.5f, 1.0f});
        }
        return b.data;
    }

    MeshData make_water_tower_data(float height, float tank_radius, Colour tank)
    {
        Builder b;
        const float tank_y0 = height * 0.6f, tank_y1 = height * 0.88f;
        const float foot = tank_radius * 0.95f, top = tank_radius * 0.7f;
        for (int leg = 0; leg < 4; ++leg)
        {
            const float sx = (leg & 1) ? 1.0f : -1.0f, sz = (leg & 2) ? 1.0f : -1.0f;
            column(b, {sx * foot, 0.03f, sz * foot}, {sx * top, tank_y0, sz * top}, 0.16f, 0.14f, 5, steel);
        }
        for (const float level : {0.22f, 0.42f})
        {
            const float f = foot + (top - foot) * level / 0.6f;
            for (int side = 0; side < 4; ++side)
            {
                const float s = (side & 1) ? 1.0f : -1.0f;
                if (side < 2)
                    column(b, {-f, height * level, s * f}, {f, height * level, s * f}, 0.07f, 0.07f, 4, dark_metal, false, false);
                else
                    column(b, {s * f, height * level, -f}, {s * f, height * level, f}, 0.07f, 0.07f, 4, dark_metal, false, false);
            }
        }
        column(b, {0.0f, tank_y0 - 0.5f, 0.0f}, {0.0f, tank_y0, 0.0f}, tank_radius * 0.55f, tank_radius, 10, shade(tank, 0.8f));
        column(b, {0.0f, tank_y0, 0.0f}, {0.0f, tank_y1, 0.0f}, tank_radius, tank_radius, 10, tank, false, false);
        column(b, {0.0f, tank_y1, 0.0f}, {0.0f, height, 0.0f}, tank_radius * 1.08f, 0.0f, 10, {96, 52, 44, 255});
        return b.data;
    }

    MeshData make_radar_data(float height, float dish_radius, Colour colour)
    {
        Builder b;
        height = std::max(height, dish_radius + 1.5f);
        box(b, {-1.4f, 0.0f, -1.4f}, {1.4f, 0.9f, 1.4f}, shade(colour, 0.7f));
        column(b, {0.0f, 0.9f, 0.0f}, {0.0f, height - 0.4f, 0.0f}, 0.34f, 0.26f, 8, steel);
        box(b, {-0.45f, height - 0.4f, -0.45f}, {0.45f, height + 0.2f, 0.45f}, dark_metal);
        const float tilt = 0.45f;
        const V axis = glm::normalize(V{0.0f, std::sin(tilt), std::cos(tilt)});
        const V hub{0.0f, height + 0.2f, 0.0f};
        const float depth = dish_radius * 0.3f;
        const V rim = hub + axis * depth;
        const Ring rim_ring = circle(rim, axis, dish_radius, 14);
        const Ring back_ring = circle(hub, axis, dish_radius * 0.12f, 14);
        const V inside = hub - axis * 2.0f;
        b.loft(back_ring, rim_ring, colour, hub + axis * 0.1f);
        const V bowl = hub + axis * (depth * 0.45f);
        for (std::size_t i = 0; i < rim_ring.size(); ++i)
            b.tri(bowl, rim_ring[i], rim_ring[(i + 1) % rim_ring.size()], shade(colour, 1.05f), inside);
        column(b, bowl, bowl + axis * (dish_radius * 0.55f), 0.07f, 0.07f, 5, dark_metal);
        box(b, bowl + axis * (dish_radius * 0.55f) - V{0.14f}, bowl + axis * (dish_radius * 0.55f) + V{0.14f}, {220, 60, 50, 255});
        return b.data;
    }

    MeshData make_domed_radar_data(float radius, Colour colour)
    {
        Builder b;
        const float plinth = radius * 0.45f;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, plinth, 0.0f}, radius * 1.05f, radius * 1.05f, 12, shade(colour, 0.7f));
        ellipsoid(b, {0.0f, plinth, 0.0f}, {radius, radius, radius}, 12, 5, 0.0f, 0.5f * pi, 0.0f, 1u, colour, 0.05f, false);
        column(b, {0.0f, plinth + radius - 0.02f, 0.0f}, {0.0f, plinth + radius + 0.35f, 0.0f}, 0.1f, 0.05f, 5, steel);
        panel(b, {0.0f, 0.8f, radius * 1.0f}, {0.0f, 0.0f, 1.0f}, radius * 0.45f, 1.6f, {68, 72, 80, 255});
        return b.data;
    }

    MeshData make_pressure_dome_data(float radius, Colour colour)
    {
        Builder b;
        radius = std::max(radius, 3.0f);
        const Colour skin = colour;
        const float ring_h = 0.6f;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, ring_h, 0.0f}, radius * 1.08f, radius * 1.08f, 24, shade(skin, 0.6f));
        column(b, {0.0f, ring_h, 0.0f}, {0.0f, ring_h + 0.15f, 0.0f}, radius * 1.0f, radius * 1.0f, 24, shade(skin, 0.45f), false, true);
        ellipsoid(b, {0.0f, ring_h, 0.0f}, {radius, radius * 0.72f, radius}, 24, 8, 0.0f, 0.5f * pi, 0.0f, 1u, skin, 0.06f, false);
        // Meridian ribs standing proud of the skin.
        for (int i = 0; i < 12; ++i)
        {
            const float a = 2.0f * pi * static_cast<float>(i) / 12.0f;
            V prev{std::sin(a) * radius * 1.01f, ring_h, std::cos(a) * radius * 1.01f};
            for (int k = 1; k <= 8; ++k)
            {
                const float lat = 0.5f * pi * static_cast<float>(k) / 8.0f;
                const V next{std::sin(a) * std::cos(lat) * radius * 1.01f, ring_h + std::sin(lat) * radius * 0.72f * 1.01f, std::cos(a) * std::cos(lat) * radius * 1.01f};
                if (glm::length(next - prev) > 1e-3f)
                    column(b, prev, next, 0.09f, 0.09f, 4, shade(skin, 0.55f), false, false);
                prev = next;
            }
        }
        // An airlock block with a door on the +Z side and a vent stack.
        const float lock_w = std::max(1.6f, radius * 0.22f);
        box(b, {-lock_w, 0.0f, radius * 0.78f}, {lock_w, lock_w * 1.25f, radius * 1.32f}, shade(skin, 0.85f));
        box(b, {-lock_w - 0.1f, lock_w * 1.25f, radius * 0.78f}, {lock_w + 0.1f, lock_w * 1.25f + 0.2f, radius * 1.34f}, shade(skin, 0.6f));
        panel(b, {0.0f, lock_w * 0.55f, radius * 1.32f + 0.02f}, {0.0f, 0.0f, 1.0f}, lock_w * 0.9f, lock_w * 1.0f, {60, 66, 78, 255});
        panel(b, {0.0f, lock_w * 1.1f, radius * 1.32f + 0.03f}, {0.0f, 0.0f, 1.0f}, lock_w * 0.5f, 0.2f, {240, 170, 60, 255});
        column(b, {radius * 0.3f, ring_h + radius * 0.7f, radius * 0.1f}, {radius * 0.3f, ring_h + radius * 0.7f + 1.2f, radius * 0.1f}, 0.18f, 0.18f, 6, steel);
        return b.data;
    }

    MeshData make_habitat_tube_data(float length, float radius, Colour colour)
    {
        Builder b;
        length = std::max(length, 4.0f);
        radius = std::max(radius, 1.0f);
        const float hl = 0.5f * length, cy = radius + 0.5f;
        for (const float z : {-0.38f, 0.38f})
            box(b, {-radius * 0.7f, 0.0f, z * length - 0.3f}, {radius * 0.7f, cy - radius * 0.6f, z * length + 0.3f}, shade(colour, 0.55f));
        column(b, {0.0f, cy, -hl}, {0.0f, cy, hl}, radius, radius, 16, colour, true, true);
        for (int i = 0; i <= 6; ++i)
        {
            const float z = -hl + length * static_cast<float>(i) / 6.0f;
            column(b, {0.0f, cy, z - 0.12f}, {0.0f, cy, z + 0.12f}, radius * 1.03f, radius * 1.03f, 16, shade(colour, 0.65f), false, false);
        }
        for (const float side : {-1.0f, 1.0f})
            for (int i = 0; i < 4; ++i)
            {
                const float z = -hl + length * (0.2f + 0.2f * static_cast<float>(i));
                panel(b, {side * (radius * 0.98f), cy + radius * 0.25f, z}, {side, 0.0f, 0.0f}, 0.6f, 0.4f, glass_dark);
            }
        panel(b, {0.0f, cy, hl + 0.02f}, {0.0f, 0.0f, 1.0f}, radius * 0.9f, radius * 1.1f, {60, 66, 78, 255});
        return b.data;
    }

    MeshData make_lunar_rover_data(float length, Colour colour)
    {
        Builder b;
        length = std::max(length, 1.5f);
        const float s = length / 3.5f, wheel = 0.45f * s;
        box(b, {-0.8f * s, 0.55f * s, -1.5f * s}, {0.8f * s, 1.05f * s, 1.5f * s}, colour);
        box(b, {-0.6f * s, 1.05f * s, -0.4f * s}, {0.6f * s, 1.55f * s, 0.9f * s}, shade(colour, 0.85f));
        panel(b, {0.0f, 1.3f * s, 0.9f * s + 0.01f}, {0.0f, 0.0f, 1.0f}, 0.9f * s, 0.35f * s, glass_dark);
        for (const float side : {-1.0f, 1.0f})
            for (const float z : {-1.2f * s, 0.0f, 1.2f * s})
            {
                const float x = side * 1.05f * s;
                column(b, {x - side * 0.15f * s, wheel, z}, {x + side * 0.15f * s, wheel, z}, wheel, wheel, 10, {44, 46, 50, 255});
                box(b, {std::min(x, side * 0.8f * s), 0.5f * s, z - 0.06f * s}, {std::max(x, side * 0.8f * s), 0.6f * s, z + 0.06f * s}, steel);
            }
        box(b, {-0.8f * s, 1.05f * s, -1.4f * s}, {0.8f * s, 1.12f * s, -0.5f * s}, {30, 48, 118, 255});
        column(b, {0.5f * s, 1.55f * s, -0.2f * s}, {0.5f * s, 2.2f * s, -0.2f * s}, 0.03f * s, 0.03f * s, 5, steel);
        ellipsoid(b, {0.5f * s, 2.2f * s, -0.2f * s}, {0.3f * s, 0.12f * s, 0.3f * s}, 10, 3, 0.0f, 0.5f * pi, 0.0f, 1u, {235, 235, 232, 255}, 0.0f);
        return b.data;
    }

    MeshData make_landing_pad_data(float radius)
    {
        Builder b;
        radius = std::max(radius, 4.0f);
        const Colour pad{96, 94, 90, 255};
        // Each layer stands a few centimetres proud of the one below so the depth buffer can separate them at a distance.
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.3f, 0.0f}, radius, radius, 32, pad);
        column(b, {0.0f, 0.3f, 0.0f}, {0.0f, 0.36f, 0.0f}, radius * 0.9f, radius * 0.9f, 32, {226, 226, 222, 255}, false, true);
        column(b, {0.0f, 0.3f, 0.0f}, {0.0f, 0.42f, 0.0f}, radius * 0.86f, radius * 0.86f, 32, shade(pad, 1.05f), false, true);
        box(b, {-radius * 0.5f, 0.42f, -radius * 0.04f}, {radius * 0.5f, 0.48f, radius * 0.04f}, {226, 226, 222, 255});
        box(b, {-radius * 0.5f, 0.42f, -radius * 0.04f}, {-radius * 0.42f, 0.48f, radius * 0.5f}, {226, 226, 222, 255});
        box(b, {radius * 0.42f, 0.42f, -radius * 0.04f}, {radius * 0.5f, 0.48f, radius * 0.5f}, {226, 226, 222, 255});
        for (int i = 0; i < 8; ++i)
        {
            const float a = 2.0f * pi * static_cast<float>(i) / 8.0f;
            const float x = std::sin(a) * radius * 0.96f, z = std::cos(a) * radius * 0.96f;
            column(b, {x, 0.3f, z}, {x, 0.65f, z}, 0.12f, 0.12f, 5, {255, 200, 90, 255});
        }
        return b.data;
    }

    MeshData make_control_tower_data(float height, std::uint32_t seed)
    {
        Builder b;
        height = std::max(height, 14.0f);
        const Colour concrete = vary({196, 194, 188, 255}, seed, 0.05f);
        const float base_h = 4.5f;
        box(b, {-7.0f, 0.0f, -4.5f}, {7.0f, base_h, 4.5f}, shade(concrete, 0.9f));
        all_facades(b, 0.0f, 0.0f, 7.0f, 4.5f, 0.0f, 1, 1, 4.5f, 2.8f, 1.8f, 1.5f, 0.3f, seed);
        panel(b, {0.0f, 1.2f, 4.5f}, {0.0f, 0.0f, 1.0f}, 2.6f, 2.4f, glass_dark);
        box(b, {-7.2f, base_h, -4.7f}, {7.2f, base_h + 0.3f, 4.7f}, shade(concrete, 0.7f));
        const float cab_y = height - 5.0f;
        column(b, {0.0f, base_h, 0.0f}, {0.0f, cab_y, 0.0f}, 2.3f, 1.8f, 8, concrete, false, false, 0.3927f);
        column(b, {0.0f, cab_y, 0.0f}, {0.0f, cab_y + 1.2f, 0.0f}, 1.8f, 3.2f, 8, shade(concrete, 0.8f), false, false, 0.3927f);
        const float glass_y = cab_y + 1.2f;
        column(b, {0.0f, glass_y, 0.0f}, {0.0f, glass_y + 2.4f, 0.0f}, 3.2f, 3.4f, 8, glass_dark, false, false, 0.3927f);
        column(b, {0.0f, glass_y + 2.4f, 0.0f}, {0.0f, glass_y + 2.8f, 0.0f}, 3.9f, 3.9f, 8, shade(concrete, 0.75f), true, true, 0.3927f);
        for (int i = 0; i < 8; ++i)
        {
            const float a0 = 2.0f * pi * static_cast<float>(i) / 8.0f + 0.3927f;
            column(b, {std::sin(a0) * 3.33f, glass_y, std::cos(a0) * 3.33f}, {std::sin(a0) * 3.43f, glass_y + 2.4f, std::cos(a0) * 3.43f}, 0.06f, 0.06f, 4,
                   concrete, false, false);
        }
        const float roof = glass_y + 2.8f;
        column(b, {0.0f, roof, 0.0f}, {0.0f, roof + 3.2f, 0.0f}, 0.1f, 0.04f, 5, steel);
        box(b, {-0.9f, roof, -0.4f}, {-0.1f, roof + 0.5f, 0.4f}, steel);
        box(b, {-0.09f, roof + 3.2f, -0.09f}, {0.09f, roof + 3.4f, 0.09f}, {255, 60, 50, 255});
        return b.data;
    }

    MeshData make_street_lamp_data(float height, Colour colour)
    {
        Builder b;
        const float arm = 0.9f;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.3f, 0.0f}, 0.16f, 0.12f, 6, shade(colour, 0.8f));
        column(b, {0.0f, 0.3f, 0.0f}, {0.0f, height, 0.0f}, 0.08f, 0.06f, 6, colour, false, true);
        box(b, {-0.04f, height - 0.04f, 0.0f}, {0.04f, height + 0.04f, arm}, colour);
        box(b, {-0.14f, height, arm - 0.3f}, {0.14f, height + 0.1f, arm + 0.3f}, shade(colour, 0.9f));
        b.quad({-0.12f, height - 0.0f, arm - 0.28f}, {0.12f, height, arm - 0.28f}, {0.12f, height, arm + 0.28f}, {-0.12f, height, arm + 0.28f},
               {255, 236, 170, 255}, {0.0f, height + 1.0f, arm});
        return b.data;
    }

    MeshData make_road_data(float length, float width)
    {
        Builder b;
        const float hw = 0.5f * width, hl = 0.5f * length;
        const float kerb = std::min(1.0f, width * 0.15f);
        const Colour asphalt{62, 63, 68, 255};
        box(b, {-hw + kerb, 0.0f, -hl}, {hw - kerb, 0.06f, hl}, asphalt);
        for (const float side : {-1.0f, 1.0f})
        {
            const float x0 = side * hw, x1 = side * (hw - kerb);
            box(b, {std::min(x0, x1), 0.0f, -hl}, {std::max(x0, x1), 0.15f, hl}, {160, 158, 152, 255});
            const float line = side * (hw - kerb - 0.25f);
            b.quad({line - 0.06f, 0.065f, -hl}, {line + 0.06f, 0.065f, -hl}, {line + 0.06f, 0.065f, hl}, {line - 0.06f, 0.065f, hl}, {220, 220, 215, 255},
                   {line, -1.0f, 0.0f});
        }
        const int dashes = std::max(1, static_cast<int>(length / 3.0f));
        for (int i = 0; i < dashes; ++i)
        {
            const float z = -hl + (static_cast<float>(i) + 0.5f) * length / static_cast<float>(dashes);
            b.quad({-0.08f, 0.065f, z - 0.7f}, {0.08f, 0.065f, z - 0.7f}, {0.08f, 0.065f, z + 0.7f}, {-0.08f, 0.065f, z + 0.7f}, {235, 220, 120, 255},
                   {0.0f, -1.0f, z});
        }
        return b.data;
    }

    // ----------------------------------------------------------------------------------------------- Small props

    MeshData make_barrel_data(float radius, float height, Colour colour)
    {
        Builder b;
        const V bottom{0.0f, 0.0f, 0.0f}, middle{0.0f, height * 0.5f, 0.0f}, top{0.0f, height, 0.0f};
        const Ring r0 = circle(bottom, {0.0f, 1.0f, 0.0f}, radius * 0.88f, 10);
        const Ring r1 = circle(middle, {0.0f, 1.0f, 0.0f}, radius, 10);
        const Ring r2 = circle(top, {0.0f, 1.0f, 0.0f}, radius * 0.88f, 10);
        b.loft(r0, r1, colour, middle);
        b.loft(r1, r2, colour, middle);
        b.cap(r0, shade(colour, 0.7f), middle);
        b.cap(r2, shade(colour, 1.15f), middle);
        for (const float level : {0.2f, 0.8f})
        {
            const float r = radius * (0.88f + 0.12f * (1.0f - std::abs(level - 0.5f) * 2.0f)) + 0.015f;
            column(b, {0.0f, height * level - 0.03f, 0.0f}, {0.0f, height * level + 0.03f, 0.0f}, r, r, 10, dark_metal, false, false);
        }
        return b.data;
    }

    MeshData make_crate_data(float size, Colour colour)
    {
        Builder b;
        const float h = 0.5f * size, t = 0.09f * size;
        box(b, {-h + 0.01f, 0.01f, -h + 0.01f}, {h - 0.01f, size - 0.01f, h - 0.01f}, colour);
        const Colour frame = shade(colour, 0.72f);
        for (const float sx : {-1.0f, 1.0f})
        {
            for (const float sz : {-1.0f, 1.0f})
            {
                const float cx = sx * (h - t * 0.5f + 0.02f), cz = sz * (h - t * 0.5f + 0.02f);
                box(b, {cx - t * 0.5f - 0.01f, 0.0f, cz - t * 0.5f - 0.01f}, {cx + t * 0.5f + 0.01f, size + 0.01f, cz + t * 0.5f + 0.01f}, frame);
            }
        }
        for (const float y : {0.0f, size - t})
        {
            for (const float s : {-1.0f, 1.0f})
            {
                const float near_edge = s > 0 ? h - t : -h, far_edge = s > 0 ? h : -h + t;
                box(b, {-h, y, near_edge}, {h, y + t, far_edge}, frame);
                box(b, {near_edge, y, -h}, {far_edge, y + t, h}, frame);
            }
        }
        return b.data;
    }

    MeshData make_container_data(float length, float width, float height, Colour colour, std::uint32_t seed)
    {
        Builder b;
        colour = vary(colour, seed, 0.18f);
        const float hl = 0.5f * length, hw = 0.5f * width;
        box(b, {-hw, 0.01f, -hl + 0.01f}, {hw, height - 0.01f, hl - 0.01f}, colour);
        const int ribs = std::max(2, static_cast<int>(length / 0.3f));
        const Colour rib = shade(colour, 0.86f);
        for (int i = 0; i < ribs; ++i)
        {
            const float z = -hl + 0.3f + (static_cast<float>(i) + 0.5f) * (length - 0.6f) / static_cast<float>(ribs);
            for (const float side : {-1.0f, 1.0f})
                box(b, {side > 0 ? hw : -hw - 0.05f, 0.12f, z - 0.06f}, {side > 0 ? hw + 0.05f : -hw, height - 0.12f, z + 0.06f}, rib);
        }
        const Colour frame = shade(colour, 0.7f);
        box(b, {-hw - 0.03f, 0.0f, -hl}, {hw + 0.03f, 0.12f, hl}, frame);
        box(b, {-hw - 0.03f, height - 0.12f, -hl}, {hw + 0.03f, height, hl}, frame);
        for (const float side : {-1.0f, 1.0f})
        {
            for (const float shift : {0.13f, 0.5f, 0.87f})
                box(b, {side * hw * shift - 0.02f, 0.12f, hl}, {side * hw * shift + 0.02f, height - 0.12f, hl + 0.04f}, steel);
        }
        panel(b, {0.0f, height * 0.5f, hl}, {0.0f, 0.0f, 1.0f}, 0.03f, height - 0.3f, dark_metal);
        return b.data;
    }

    MeshData make_tank_data(float radius, float height, Colour colour)
    {
        Builder b;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.3f, 0.0f}, radius * 1.04f, radius * 1.04f, 14, shade(colour, 0.6f));
        column(b, {0.0f, 0.3f, 0.0f}, {0.0f, height, 0.0f}, radius, radius, 14, colour, false, false);
        ellipsoid(b, {0.0f, height, 0.0f}, {radius, radius * 0.3f, radius}, 14, 3, 0.0f, 0.5f * pi, 0.0f, 3u, shade(colour, 1.05f), 0.0f);
        const float lx = 0.0f, lz = radius + 0.1f;
        for (const float side : {-1.0f, 1.0f})
            box(b, {lx + side * 0.25f - 0.03f, 0.3f, lz - 0.05f}, {lx + side * 0.25f + 0.03f, height, lz + 0.03f}, steel);
        for (int i = 1; i < static_cast<int>(height / 0.5f); ++i)
            box(b, {-0.25f, 0.3f + 0.5f * static_cast<float>(i) - 0.02f, lz - 0.04f}, {0.25f, 0.3f + 0.5f * static_cast<float>(i) + 0.02f, lz + 0.0f}, steel);
        column(b, {0.0f, 0.8f, -radius + 0.1f}, {0.0f, 0.8f, -radius - 1.2f}, 0.16f, 0.16f, 6, steel);
        column(b, {0.0f, 0.0f, -radius - 1.2f}, {0.0f, 0.8f, -radius - 1.2f}, 0.16f, 0.16f, 6, steel, false, false);
        return b.data;
    }

    // ----------------------------------------------------------------------------------------------- Nature

    MeshData make_tree_data(float height, float radius, Colour trunk, Colour foliage, std::uint32_t seed)
    {
        Builder b;
        trunk = vary(trunk, seed, 0.1f);
        foliage = vary(foliage, seed + 3u, 0.12f);
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, height * 0.6f, 0.0f}, radius * 0.17f, radius * 0.09f, 6, trunk, false, false);
        const float cy = height * 0.64f, ry = height * 0.32f;
        ellipsoid(b, {0.0f, cy, 0.0f}, {radius, ry, radius}, 8, 4, -0.5f * pi, 0.5f * pi, 0.18f, seed, foliage, 0.1f);
        for (int blob = 0; blob < 2; ++blob)
        {
            const float angle = 2.0f * pi * hash01(seed * 5u + static_cast<std::uint32_t>(blob)) + static_cast<float>(blob) * pi;
            ellipsoid(b, {std::sin(angle) * radius * 0.55f, cy - ry * 0.25f + static_cast<float>(blob) * ry * 0.5f, std::cos(angle) * radius * 0.55f},
                      {radius * 0.62f, ry * 0.7f, radius * 0.62f}, 7, 3, -0.5f * pi, 0.5f * pi, 0.2f, seed + 9u + static_cast<std::uint32_t>(blob),
                      shade(foliage, 0.92f), 0.1f);
        }
        return b.data;
    }

    MeshData make_conifer_data(float height, float radius, Colour trunk, Colour foliage, int sides)
    {
        sides = std::max(sides, 3);
        MeshData data;
        add_cylinder(data, 0.0f, height * 0.3f, radius * 0.18f, std::min(sides, 6), trunk);
        add_cone(data, height * 0.18f, height * 0.5f, radius, sides, shade(foliage, 0.85f));
        add_cone(data, height * 0.45f, height * 0.55f, radius * 0.7f, sides, foliage);
        return data;
    }

    MeshData make_rock_data(float radius, Colour colour, std::uint32_t seed)
    {
        constexpr int segments = 7;
        constexpr int rings = 4;
        // A displaced, flattened half-sphere of flat-shaded facets.
        std::vector<glm::vec3> points;
        for (int ring = 0; ring <= rings; ++ring)
        {
            const float elevation = 0.5f * pi * static_cast<float>(ring) / static_cast<float>(rings);
            for (int segment = 0; segment < segments; ++segment)
            {
                const float angle = 2.0f * pi * static_cast<float>(segment) / static_cast<float>(segments);
                const float jitter = 0.78f + 0.44f * hash01(seed * 131u + static_cast<std::uint32_t>(ring * 17 + segment));
                const float r = radius * jitter;
                points.push_back({std::cos(elevation) * std::sin(angle) * r, std::sin(elevation) * r * 0.7f,
                                  std::cos(elevation) * std::cos(angle) * r});
            }
        }
        MeshData data;
        for (int ring = 0; ring < rings; ++ring)
        {
            for (int segment = 0; segment < segments; ++segment)
            {
                const int next = (segment + 1) % segments;
                const glm::vec3 &p00 = points[static_cast<std::size_t>(ring * segments + segment)];
                const glm::vec3 &p01 = points[static_cast<std::size_t>(ring * segments + next)];
                const glm::vec3 &p10 = points[static_cast<std::size_t>((ring + 1) * segments + segment)];
                const glm::vec3 &p11 = points[static_cast<std::size_t>((ring + 1) * segments + next)];
                const Colour facet = shade(colour, 0.85f + 0.3f * hash01(seed + static_cast<std::uint32_t>(ring * 31 + segment)));
                add_flat_triangle(data, p00, p01, p11, facet);
                add_flat_triangle(data, p00, p11, p10, facet);
            }
        }
        return data;
    }

    MeshData make_bush_data(float radius, Colour colour, std::uint32_t seed)
    {
        Builder b;
        colour = vary(colour, seed, 0.12f);
        ellipsoid(b, {0.0f, 0.0f, 0.0f}, {radius, radius * 0.8f, radius}, 7, 3, 0.0f, 0.5f * pi, 0.2f, seed, colour, 0.12f, true);
        ellipsoid(b, {radius * 0.6f, 0.0f, radius * 0.2f}, {radius * 0.6f, radius * 0.6f, radius * 0.6f}, 6, 3, 0.0f, 0.5f * pi, 0.2f,
                  seed + 4u, shade(colour, 0.9f), 0.12f, true);
        return b.data;
    }

    MeshData make_rock_cluster_data(float radius, Colour colour, std::uint32_t seed)
    {
        Builder b;
        const int count = 3 + static_cast<int>(hash01(seed) * 2.99f);
        for (int i = 0; i < count; ++i)
        {
            const auto id = seed * 13u + static_cast<std::uint32_t>(i);
            const float angle = 2.0f * pi * (static_cast<float>(i) + hash01(id)) / static_cast<float>(count);
            const float size = (i == 0 ? 0.55f : 0.25f + 0.25f * hash01(id + 7u)) * radius;
            const float spread = i == 0 ? 0.0f : radius * (0.45f + 0.3f * hash01(id + 3u));
            b.append(make_rock_data(size, vary(colour, id, 0.1f), id), {std::sin(angle) * spread, 0.0f, std::cos(angle) * spread},
                     2.0f * pi * hash01(id + 11u), 1.0f);
        }
        return b.data;
    }

    // ----------------------------------------------------------------------------------------------- Vehicles

    MeshData make_car_data(float length, Colour body, std::uint32_t seed)
    {
        Builder b;
        body = vary(body, seed, 0.1f);
        const float k = length / 4.4f;
        const float hw = 0.9f * k, hl = 0.5f * length, wheel_r = 0.33f * k;
        const Colour tyre{26, 26, 30, 255};
        box(b, {-hw, 0.3f * k, -hl}, {hw, 0.85f * k, hl}, body);
        const Ring cab_lo = rect(0.0f, -0.12f * k, hw - 0.08f * k, 1.2f * k, 0.85f * k);
        const Ring cab_hi = rect(0.0f, -0.28f * k, hw - 0.22f * k, 0.78f * k, 1.4f * k);
        const V inside{0.0f, 1.0f * k, -0.2f * k};
        b.loft(cab_lo, cab_hi, glass_dark, inside);
        b.cap(cab_hi, shade(body, 1.05f), inside);
        for (const float sx : {-1.0f, 1.0f})
        {
            for (const float z : {-0.3f * length, 0.3f * length})
            {
                const float x = sx * (hw - 0.04f * k);
                column(b, {x - 0.14f * k, wheel_r, z}, {x + 0.14f * k, wheel_r, z}, wheel_r, wheel_r, 8, tyre);
                column(b, {x + sx * 0.12f * k, wheel_r, z}, {x + sx * 0.16f * k, wheel_r, z}, wheel_r * 0.55f, wheel_r * 0.55f, 8, {170, 172, 178, 255});
                box(b, {x - 0.17f * k, wheel_r * 1.2f, z - wheel_r * 1.35f}, {x + 0.17f * k, 0.5f * k, z + wheel_r * 1.35f}, shade(body, 0.55f));
            }
            panel(b, {sx * hw * 0.6f, 0.62f * k, hl}, {0.0f, 0.0f, 1.0f}, 0.38f * k, 0.16f * k, {255, 244, 200, 255});
            panel(b, {sx * hw * 0.6f, 0.65f * k, -hl}, {0.0f, 0.0f, -1.0f}, 0.34f * k, 0.14f * k, {220, 30, 30, 255});
        }
        box(b, {-hw, 0.2f * k, hl - 0.05f * k}, {hw, 0.34f * k, hl + 0.08f * k}, steel);
        box(b, {-hw, 0.2f * k, -hl - 0.08f * k}, {hw, 0.34f * k, -hl + 0.05f * k}, steel);
        return b.data;
    }

    MeshData make_spaceship_data(float length, Colour hull)
    {
        Builder b;
        const float R = length * 0.14f, leg_h = R * 0.55f, cy = leg_h + R * 0.9f + 0.1f;
        const float L = length;
        struct Station
        {
            float z, rx, ry;
        };
        const Station stations[] = {{-0.5f, 0.55f, 0.5f}, {-0.36f, 0.8f, 0.75f}, {-0.1f, 1.0f, 0.9f}, {0.2f, 0.85f, 0.72f}, {0.38f, 0.45f, 0.36f}, {0.5f, 0.03f, 0.03f}};
        constexpr int sides = 8;
        constexpr float phase = pi / 8.0f;
        Ring previous;
        for (const Station &station : stations)
        {
            Ring ring = ring_xy(station.z * L, cy, R * station.rx, R * station.ry, sides, phase);
            if (!previous.empty())
                b.loft(previous, ring, [&](int i)
                       { return std::sin(phase + 2.0f * pi * (static_cast<float>(i) + 0.5f) / static_cast<float>(sides)) < 0.0f ? shade(hull, 0.75f) : hull; },
                       {0.0f, cy, station.z * L});
            else
                b.cap(ring, dark_metal, {0.0f, cy, station.z * L + 1.0f});
            previous = ring;
        }
        // Canopy.
        ellipsoid(b, {0.0f, cy + R * 0.45f, L * 0.2f}, {R * 0.5f, R * 0.42f, L * 0.13f}, 8, 3, -0.15f * pi, 0.5f * pi, 0.0f, 2u, {96, 190, 230, 255}, 0.05f);
        // Swept wings, thin slabs.
        for (const float s : {-1.0f, 1.0f})
        {
            const float y = cy - R * 0.35f;
            const Ring lo{{s * R * 0.7f, y, 0.15f * L}, {s * L * 0.48f, y - 0.1f * R, -0.3f * L}, {s * L * 0.48f, y - 0.1f * R, -0.4f * L},
                          {s * R * 0.7f, y, -0.36f * L}};
            Ring hi = lo;
            for (V &p : hi)
                p.y += R * 0.14f;
            b.prism(lo, hi, shade(hull, 0.9f));
            box(b, {s * L * 0.48f - 0.08f, y - 0.1f * R, -0.4f * L}, {s * L * 0.48f + 0.08f, y + 0.05f * R, -0.3f * L}, {230, 80, 60, 255});
            // Engine pod with a glowing exhaust.
            const float ex = s * R * 1.5f;
            column(b, {ex, y - 0.15f * R, -0.08f * L}, {ex, y - 0.15f * R, -0.5f * L}, R * 0.28f, R * 0.22f, 8, shade(hull, 0.65f), true, false);
            const Ring glow = circle({ex, y - 0.15f * R, -0.5f * L}, {0.0f, 0.0f, 1.0f}, R * 0.2f, 8);
            b.cap(glow, {120, 210, 255, 255}, {ex, y - 0.15f * R, -0.2f * L});
        }
        // Tail fin.
        {
            const float x = 0.06f * R;
            const Ring lo{{-x, cy + R * 0.7f, -0.12f * L}, {-x, cy + R * 2.6f, -0.42f * L}, {-x, cy + R * 2.6f, -0.5f * L}, {-x, cy + R * 0.3f, -0.5f * L}};
            Ring hi = lo;
            for (V &p : hi)
                p.x = x;
            b.prism(lo, hi, {230, 80, 60, 255});
        }
        const Ring core = circle({0.0f, cy, -0.5f * L - 0.02f}, {0.0f, 0.0f, 1.0f}, R * 0.4f, 8, phase);
        b.cap(core, {150, 225, 255, 255}, {0.0f, cy, -0.3f * L});
        // Landing legs.
        const V feet[] = {{-R * 0.75f, 0.0f, -0.18f * L}, {R * 0.75f, 0.0f, -0.18f * L}, {0.0f, 0.0f, 0.26f * L}};
        for (const V &foot : feet)
        {
            column(b, {foot.x, 0.04f, foot.z}, {foot.x * 0.8f, cy - R * 0.5f, foot.z}, R * 0.08f, R * 0.06f, 5, steel, false, false);
            column(b, {foot.x, 0.0f, foot.z}, {foot.x, 0.06f, foot.z}, R * 0.28f, R * 0.26f, 8, dark_metal);
        }
        return b.data;
    }

    MeshData make_aeroplane_data(float wingspan, Colour body, Colour stripe)
    {
        Builder b;
        const float L = wingspan * 0.95f, R = wingspan * 0.07f;
        const float cy = R * 2.6f;
        struct Station
        {
            float z, r;
        };
        const Station stations[] = {{-0.5f, 0.08f}, {-0.4f, 0.4f}, {-0.2f, 1.0f}, {0.26f, 1.0f}, {0.4f, 0.76f}, {0.5f, 0.12f}};
        constexpr int sides = 8;
        constexpr float phase = pi / 8.0f;
        Ring previous;
        for (const Station &station : stations)
        {
            const Ring ring = ring_xy(station.z * L, station.z < -0.3f ? cy + R * 0.25f : cy, R * station.r, R * station.r, sides, phase);
            if (!previous.empty())
                b.loft(previous, ring, [&](int i)
                       {
                           const float a = phase + 2.0f * pi * (static_cast<float>(i) + 0.5f) / static_cast<float>(sides);
                           return std::sin(a) < -0.4f ? stripe : body;
                       },
                       {0.0f, cy, station.z * L});
            previous = ring;
        }
        const float flat = R * std::cos(pi / 8.0f);
        for (const float s : {-1.0f, 1.0f})
        {
            for (int i = 0; i < 13; ++i)
                panel(b, {s * flat, cy + R * 0.15f, (-0.18f + 0.032f * static_cast<float>(i)) * L}, {s, 0.0f, 0.0f}, L * 0.02f, R * 0.3f, glass_dark);
            panel(b, {s * R * 0.3f, cy + R * 0.28f, 0.42f * L}, {s * 0.6f, 0.2f, 0.7f}, R * 0.5f, R * 0.25f, glass_dark, {0.0f, 1.0f, 0.0f});
        }
        // Wings, tail planes and engines.
        for (const float s : {-1.0f, 1.0f})
        {
            const float y = cy - R * 0.5f;
            const Ring lo{{s * R * 0.9f, y, 0.06f * L}, {s * wingspan * 0.5f, y + R * 0.5f, -0.14f * L}, {s * wingspan * 0.5f, y + R * 0.5f, -0.2f * L},
                          {s * R * 0.9f, y, -0.14f * L}};
            Ring hi = lo;
            for (V &p : hi)
                p.y += R * 0.12f;
            b.prism(lo, hi, shade(body, 0.95f));
            const Ring tail_lo{{s * R * 0.4f, cy + R * 0.3f, -0.4f * L}, {s * wingspan * 0.2f, cy + R * 0.55f, -0.47f * L},
                               {s * wingspan * 0.2f, cy + R * 0.55f, -0.5f * L}, {s * R * 0.4f, cy + R * 0.3f, -0.5f * L}};
            Ring tail_hi = tail_lo;
            for (V &p : tail_hi)
                p.y += R * 0.08f;
            b.prism(tail_lo, tail_hi, shade(body, 0.95f));
            const float ex = s * wingspan * 0.19f, ey = y - R * 0.5f;
            column(b, {ex, ey, 0.06f * L}, {ex, ey, -0.1f * L}, R * 0.42f, R * 0.4f, 8, shade(body, 0.9f), false, true);
            const Ring intake = circle({ex, ey, 0.06f * L}, {0.0f, 0.0f, 1.0f}, R * 0.34f, 8);
            b.cap(intake, dark_metal, {ex, ey, -0.05f * L});
            column(b, {ex, ey + R * 0.4f, 0.0f}, {ex, y + R * 0.1f, 0.0f}, R * 0.06f, R * 0.06f, 4, steel, false, false);
        }
        {
            const Ring lo{{-R * 0.07f, cy + R * 0.8f, -0.3f * L}, {-R * 0.07f, cy + R * 3.2f, -0.47f * L}, {-R * 0.07f, cy + R * 3.2f, -0.5f * L},
                          {-R * 0.07f, cy + R * 0.5f, -0.5f * L}};
            Ring hi = lo;
            for (V &p : hi)
                p.x = R * 0.07f;
            b.prism(lo, hi, stripe);
        }
        // Landing gear: nose gear and a main gear under the wing roots.
        const float wheel_r = R * 0.32f;
        const V gear[] = {{0.0f, 0.0f, 0.38f * L}, {-R * 1.0f, 0.0f, -0.02f * L}, {R * 1.0f, 0.0f, -0.02f * L}};
        for (const V &g : gear)
        {
            column(b, {g.x, wheel_r, g.z}, {g.x * 0.8f, cy - R * 0.6f, g.z}, R * 0.07f, R * 0.07f, 4, steel, false, false);
            column(b, {g.x - wheel_r * 0.4f, wheel_r, g.z}, {g.x + wheel_r * 0.4f, wheel_r, g.z}, wheel_r, wheel_r, 8, {26, 26, 30, 255});
        }
        return b.data;
    }

    // ---- Control parts: panels, dials, knobs, switches, lamps and meters --------------------------------------------
    namespace
    {
        constexpr float tau = 2.0f * pi;

        /** A flat quad lying in the z = `z` plane, `w` wide and `h` long along `up` (an xy direction). */
        void flat(Builder &b, float cx, float cy, float z, float w, float h, float up_x, float up_y, Colour colour)
        {
            const V up{up_x, up_y, 0.0f};
            const V right{up_y, -up_x, 0.0f};
            const V c{cx, cy, z};
            b.quad(c - right * (0.5f * w) - up * (0.5f * h), c + right * (0.5f * w) - up * (0.5f * h), c + right * (0.5f * w) + up * (0.5f * h),
                   c - right * (0.5f * w) + up * (0.5f * h), colour, V{cx, cy, z - 1.0f});
        }

        /** The side wall of a cylinder about the z axis, facing outward or inward. */
        void ring_wall(Builder &b, float z0, float z1, float r, int sides, Colour colour, bool inward)
        {
            for (int i = 0; i < sides; ++i)
            {
                const float a0 = tau * static_cast<float>(i) / static_cast<float>(sides);
                const float a1 = tau * static_cast<float>(i + 1) / static_cast<float>(sides);
                const V p{r * std::cos(a0), r * std::sin(a0), z0}, q{r * std::cos(a1), r * std::sin(a1), z0};
                const V mid{0.5f * (p.x + q.x), 0.5f * (p.y + q.y), 0.5f * (z0 + z1)};
                const V radial = glm::normalize(V{mid.x, mid.y, 0.0f});
                b.quad(p, q, V{q.x, q.y, z1}, V{p.x, p.y, z1}, colour, inward ? mid + radial * 10.0f : V{0.0f, 0.0f, mid.z});
            }
        }

        /** An annulus in the z = `z` plane facing +Z. */
        void ring_flat(Builder &b, float z, float r0, float r1, int sides, Colour colour)
        {
            for (int i = 0; i < sides; ++i)
            {
                const float a0 = tau * static_cast<float>(i) / static_cast<float>(sides);
                const float a1 = tau * static_cast<float>(i + 1) / static_cast<float>(sides);
                b.quad({r0 * std::cos(a0), r0 * std::sin(a0), z}, {r0 * std::cos(a1), r0 * std::sin(a1), z},
                       {r1 * std::cos(a1), r1 * std::sin(a1), z}, {r1 * std::cos(a0), r1 * std::sin(a0), z}, colour, V{0.0f, 0.0f, z - 1.0f});
            }
        }

        /** A hollow rim: outer wall, top and inner wall, between radii r0 and r1 and heights z0 to z1. */
        void rim(Builder &b, float z0, float z1, float r0, float r1, int sides, Colour colour)
        {
            ring_wall(b, z0, z1, r1, sides, colour, false);
            ring_flat(b, z1, r0, r1, sides, shade(colour, 1.25f));
            ring_wall(b, z0, z1, r0, sides, shade(colour, 0.6f), true);
        }

        /** Tick marks on a dial or knob scale: `count` marks swept clockwise from the top over `sweep` degrees. */
        void scale_ticks(Builder &b, float r_outer, float length, float width, int count, float sweep_degrees, float z, Colour colour,
                         Colour last)
        {
            for (int i = 0; i < count; ++i)
            {
                const float t = count > 1 ? static_cast<float>(i) / static_cast<float>(count - 1) : 0.5f;
                const float angle = (-0.5f + t) * sweep_degrees * pi / 180.0f;
                const float dx = std::sin(angle), dy = std::cos(angle);
                const float mid = r_outer - 0.5f * length;
                flat(b, dx * mid, dy * mid, z, width, length, dx, dy, i == count - 1 ? last : colour);
                if (i + 1 < count)
                {
                    const float half = angle + 0.5f * sweep_degrees * pi / 180.0f / static_cast<float>(count - 1);
                    const float hx = std::sin(half), hy = std::cos(half);
                    const float minor_mid = r_outer - 0.25f * length;
                    flat(b, hx * minor_mid, hy * minor_mid, z, width * 0.6f, length * 0.5f, hx, hy, colour);
                }
            }
        }

        bool is_light(Colour colour)
        {
            return colour.red + colour.green + colour.blue > 3 * 140;
        }
    } // namespace

    MeshData make_panel_data(float width, float height, float depth, Colour colour)
    {
        Builder b;
        const float hw = 0.5f * width, hh = 0.5f * height;
        box(b, {-hw, -hh, -depth}, {hw, hh, 0.0f}, colour);
        const float t = 0.018f * std::min(width, height);
        const float lip = 0.35f * t;
        const Colour frame = shade(colour, 0.62f);
        box(b, {-hw, hh - t, 0.0f}, {hw, hh, lip}, frame);
        box(b, {-hw, -hh, 0.0f}, {hw, -hh + t, lip}, frame);
        box(b, {-hw, -hh + t, 0.0f}, {-hw + t, hh - t, lip}, frame);
        box(b, {hw - t, -hh + t, 0.0f}, {hw, hh - t, lip}, frame);
        const float screw_r = 0.28f * t;
        for (const float sx : {-1.0f, 1.0f})
            for (const float sy : {-1.0f, 1.0f})
            {
                const float x = sx * (hw - 2.2f * t), y = sy * (hh - 2.2f * t);
                column(b, {x, y, 0.0f}, {x, y, 0.5f * t}, screw_r, screw_r * 0.9f, 8, steel);
                const float slot = (sx * sy > 0.0f) ? 0.7f : -0.7f;
                flat(b, x, y, 0.5f * t + 0.0004f, screw_r * 0.3f, screw_r * 1.7f, slot, 0.7f, dark_metal);
            }
        return b.data;
    }

    MeshData make_dial_data(float radius, int ticks, float sweep_degrees, Colour face)
    {
        Builder b;
        const float r = radius;
        const int sides = 28;
        const float face_z = 0.03f * r, rim_h = 0.11f * r;
        rim(b, 0.0f, rim_h, r, 1.14f * r, sides, steel);
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, face_z}, r, r, sides, face, false, true);
        const Colour ink = is_light(face) ? Colour{30, 32, 36, 255} : Colour{232, 232, 224, 255};
        scale_ticks(b, 0.9f * r, 0.17f * r, 0.03f * r, std::max(ticks, 2), sweep_degrees, face_z + 0.0006f, ink, {220, 52, 40, 255});
        return b.data;
    }

    MeshData make_needle_data(float radius, Colour colour)
    {
        Builder b;
        const float r = radius, w = 0.035f * r, z0 = 0.045f * r, z1 = z0 + 0.012f * r;
        const auto outline = [&](float z) -> Ring
        {
            return {{-w, -0.2f * r, z}, {w, -0.2f * r, z}, {0.4f * w, 0.78f * r, z}, {0.0f, 0.86f * r, z}, {-0.4f * w, 0.78f * r, z}};
        };
        b.prism(outline(z0), outline(z1), colour);
        column(b, {0.0f, 0.0f, z0}, {0.0f, 0.0f, z0 + 0.06f * r}, 0.1f * r, 0.08f * r, 10, dark_metal);
        return b.data;
    }

    MeshData make_knob_base_data(float radius, int ticks, float sweep_degrees)
    {
        Builder b;
        const float r = radius;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.006f}, 1.12f * r, 1.12f * r, 20, dark_metal, false, true);
        scale_ticks(b, 1.5f * r, 0.2f * r, 0.035f * r, std::max(ticks, 2), sweep_degrees, 0.0012f, {226, 226, 218, 255}, {226, 226, 218, 255});
        return b.data;
    }

    MeshData make_knob_data(float radius, float height, Colour colour)
    {
        Builder b;
        const float r = radius, h = height;
        const int sides = 16;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.4f * h}, r, 0.94f * r, sides, shade(colour, 0.8f), false, false);
        column(b, {0.0f, 0.0f, 0.4f * h}, {0.0f, 0.0f, 0.9f * h}, 0.94f * r, 0.84f * r, sides, colour, false, false);
        column(b, {0.0f, 0.0f, 0.9f * h}, {0.0f, 0.0f, h}, 0.84f * r, 0.7f * r, sides, shade(colour, 1.15f), false, true);
        // The pointer line, so the rotation reads.
        flat(b, 0.0f, 0.4f * r, h + 0.0004f, 0.1f * r, 0.62f * r, 0.0f, 1.0f, {240, 240, 232, 255});
        return b.data;
    }

    MeshData make_toggle_base_data(float size)
    {
        Builder b;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.2f * size}, 0.62f * size, 0.62f * size, 6, steel);
        column(b, {0.0f, 0.0f, 0.2f * size}, {0.0f, 0.0f, 0.34f * size}, 0.34f * size, 0.3f * size, 10, shade(steel, 1.25f));
        return b.data;
    }

    MeshData make_toggle_lever_data(float size)
    {
        Builder b;
        const float pivot = 0.3f * size;
        column(b, {0.0f, 0.0f, pivot}, {0.0f, 0.0f, pivot + 0.75f * size}, 0.12f * size, 0.085f * size, 8, shade(steel, 1.4f), false, false);
        column(b, {0.0f, 0.0f, pivot + 0.7f * size}, {0.0f, 0.0f, pivot + 0.82f * size}, 0.14f * size, 0.17f * size, 10, {200, 200, 206, 255}, false, false);
        column(b, {0.0f, 0.0f, pivot + 0.82f * size}, {0.0f, 0.0f, pivot + 0.98f * size}, 0.17f * size, 0.1f * size, 10, {212, 212, 218, 255}, false, true);
        return b.data;
    }

    MeshData make_button_base_data(float radius)
    {
        Builder b;
        rim(b, 0.0f, 0.22f * radius, 0.9f * radius, 1.2f * radius, 20, dark_metal);
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.02f * radius}, 0.9f * radius, 0.9f * radius, 20, {18, 18, 20, 255}, false, true);
        return b.data;
    }

    MeshData make_button_cap_data(float radius, Colour colour)
    {
        Builder b;
        const float r = radius;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.34f * r}, 0.84f * r, 0.84f * r, 20, shade(colour, 0.75f), false, false);
        column(b, {0.0f, 0.0f, 0.34f * r}, {0.0f, 0.0f, 0.44f * r}, 0.84f * r, 0.66f * r, 20, colour, false, true);
        return b.data;
    }

    MeshData make_lamp_bezel_data(float radius)
    {
        Builder b;
        rim(b, 0.0f, 0.24f * radius, 0.96f * radius, 1.22f * radius, 16, steel);
        return b.data;
    }

    MeshData make_lamp_lens_data(float radius)
    {
        Builder b;
        const float r = radius;
        const float zs[] = {0.0f, 0.1f * r, 0.2f * r, 0.27f * r, 0.31f * r};
        const float rs[] = {0.98f * r, 0.95f * r, 0.82f * r, 0.55f * r, 0.25f * r};
        for (int i = 0; i + 1 < 5; ++i)
            column(b, {0.0f, 0.0f, zs[i]}, {0.0f, 0.0f, zs[i + 1]}, rs[i], rs[i + 1], 14, {255, 255, 255, 255}, false, i == 3);
        return b.data;
    }

    MeshData make_slider_track_data(float length, float width)
    {
        Builder b;
        const float half = 0.5f * length;
        box(b, {-0.5f * width, -half - 0.4f * width, 0.0f}, {0.5f * width, half + 0.4f * width, 0.004f}, dark_metal);
        box(b, {-0.11f * width, -half, 0.004f}, {0.11f * width, half, 0.0055f}, {10, 10, 12, 255});
        for (int i = 0; i <= 10; ++i)
        {
            const float y = -half + length * static_cast<float>(i) / 10.0f;
            const float tick = (i % 5 == 0) ? 0.3f : 0.18f;
            flat(b, -0.38f * width, y, 0.0045f, tick * width, 0.02f * width, 1.0f, 0.0f, {220, 220, 212, 255});
            flat(b, 0.38f * width, y, 0.0045f, tick * width, 0.02f * width, 1.0f, 0.0f, {220, 220, 212, 255});
        }
        return b.data;
    }

    MeshData make_slider_cap_data(float width)
    {
        Builder b;
        const float w = width;
        box(b, {-0.42f * w, -0.3f * w, 0.0f}, {0.42f * w, 0.3f * w, 0.3f * w}, {58, 60, 66, 255});
        box(b, {-0.4f * w, -0.28f * w, 0.3f * w}, {0.4f * w, 0.28f * w, 0.36f * w}, {80, 82, 90, 255});
        flat(b, 0.0f, 0.0f, 0.3626f * w, 0.7f * w, 0.05f * w, 1.0f, 0.0f, {240, 240, 232, 255});
        return b.data;
    }

    MeshData make_meter_housing_data(float width, float height)
    {
        Builder b;
        const float t = 0.12f * std::min(width, height);
        box(b, {-0.5f * width - t, -0.5f * height - t, 0.0f}, {0.5f * width + t, 0.5f * height + t, 0.006f}, dark_metal);
        box(b, {-0.5f * width, -0.5f * height, 0.006f}, {0.5f * width, 0.5f * height, 0.0075f}, {8, 8, 10, 255});
        return b.data;
    }

    MeshData make_meter_segment_data(float width, float height)
    {
        Builder b;
        box(b, {-0.5f * width, -0.5f * height, 0.0075f}, {0.5f * width, 0.5f * height, 0.0105f}, {255, 255, 255, 255});
        return b.data;
    }

    void transform_mesh_data(MeshData &data, const glm::mat4 &matrix)
    {
        const glm::mat3 rotation(matrix);
        for (Vertex3D &vertex : data.vertices)
        {
            const glm::vec4 p = matrix * glm::vec4(vertex.position[0], vertex.position[1], vertex.position[2], 1.0f);
            const V n = glm::normalize(rotation * V(vertex.normal[0], vertex.normal[1], vertex.normal[2]));
            vertex.position[0] = p.x;
            vertex.position[1] = p.y;
            vertex.position[2] = p.z;
            vertex.normal[0] = n.x;
            vertex.normal[1] = n.y;
            vertex.normal[2] = n.z;
        }
    }

    MeshData make_cockpit_data(float panel_width, float panel_height, glm::mat4 *panel_pose)
    {
        // Assembly frame: floor at y = 0, the pilot sits at +Z looking toward -Z at a dashboard tilted back 20 degrees.
        const float tilt = -20.0f * pi / 180.0f;
        const glm::mat4 pose = glm::translate(glm::mat4(1.0f), V{0.0f, 0.95f, -0.45f}) * glm::rotate(glm::mat4(1.0f), tilt, V{1.0f, 0.0f, 0.0f});
        if (panel_pose)
            *panel_pose = pose;
        const float hw = 0.5f * panel_width, hh = 0.5f * panel_height;
        const Colour trim{38, 40, 46, 255};
        const Colour shell{64, 68, 76, 255};

        MeshData out;
        {
            Builder local;
            box(local, {-hw - 0.12f, -hh - 0.08f, -0.7f}, {hw + 0.12f, hh + 0.04f, -0.03f}, shell);
            box(local, {-hw - 0.14f, hh + 0.04f, -0.7f}, {hw + 0.14f, hh + 0.11f, 0.2f}, trim); // glare shield
            box(local, {-hw - 0.14f, hh + 0.02f, 0.2f}, {hw + 0.14f, hh + 0.11f, 0.26f}, shade(trim, 0.8f)); // lip, clear of the shield top
            transform_mesh_data(local.data, pose);
            append_mesh_data(out, local.data);
        }
        Builder b;
        box(b, {-1.0f, 0.0f, -1.0f}, {1.0f, 0.05f, 1.2f}, {50, 52, 58, 255});               // floor
        box(b, {-0.6f, 0.05f, -1.0f}, {0.6f, 0.62f, -0.4f}, shade(shell, 0.85f));           // pedestal
        box(b, {-1.0f, 0.05f, -1.0f}, {-0.8f, 0.7f, 0.55f}, shade(shell, 0.9f));            // side consoles
        box(b, {0.8f, 0.05f, -1.0f}, {1.0f, 0.7f, 0.55f}, shade(shell, 0.9f));
        box(b, {-0.96f, 0.7f, -0.9f}, {-0.78f, 0.74f, 0.45f}, trim);
        box(b, {0.78f, 0.7f, -0.9f}, {0.96f, 0.74f, 0.45f}, trim);
        // Pilot seat, stick and throttle quadrant stub.
        box(b, {-0.3f, 0.3f, 0.45f}, {0.3f, 0.42f, 0.95f}, {58, 62, 90, 255});
        box(b, {-0.3f, 0.42f, 0.9f}, {0.3f, 0.95f, 1.0f}, {58, 62, 90, 255});
        box(b, {-0.2f, 0.95f, 0.9f}, {0.2f, 1.1f, 0.98f}, {50, 54, 80, 255});
        box(b, {-0.1f, 0.05f, 0.5f}, {0.1f, 0.3f, 0.85f}, trim);
        column(b, {0.0f, 0.62f, 0.15f}, {0.0f, 1.0f, 0.22f}, 0.025f, 0.02f, 8, steel);
        column(b, {0.0f, 1.0f, 0.22f}, {0.0f, 1.08f, 0.23f}, 0.045f, 0.04f, 8, trim);
        // Canopy frame: side pillars leaning inward, a roof spar and a windscreen bow.
        for (const float side : {-1.0f, 1.0f})
        {
            column(b, {side * 0.98f, 0.7f, 0.9f}, {side * 0.55f, 1.95f, 0.1f}, 0.04f, 0.035f, 6, shade(shell, 1.1f));
            column(b, {side * 0.98f, 0.74f, -0.9f}, {side * 0.5f, 1.78f, -0.55f}, 0.035f, 0.03f, 6, shade(shell, 1.1f));
            column(b, {side * 0.5f, 1.78f, -0.55f}, {side * 0.55f, 1.95f, 0.1f}, 0.03f, 0.035f, 6, shade(shell, 1.1f));
        }
        column(b, {-0.5f, 1.78f, -0.55f}, {0.5f, 1.78f, -0.55f}, 0.03f, 0.03f, 6, shade(shell, 1.1f));
        column(b, {-0.55f, 1.95f, 0.1f}, {0.55f, 1.95f, 0.1f}, 0.035f, 0.035f, 6, shade(shell, 1.1f));
        append_mesh_data(out, b.data);
        return out;
    }

    MeshData make_control_cabinet_data(float panel_width, float panel_height, glm::mat4 *panel_pose)
    {
        const float depth = 0.45f, half_w = 0.5f * std::max(0.8f, panel_width + 0.16f), body_h = 1.9f;
        const float door_z = 0.5f * depth;
        if (panel_pose)
            *panel_pose = glm::translate(glm::mat4(1.0f), V{0.0f, 1.2f, door_z + 0.03f});
        const Colour paint{116, 128, 136, 255};
        Builder b;
        box(b, {-half_w, 0.08f, -0.5f * depth}, {half_w, body_h, door_z - 0.03f}, shade(paint, 0.8f));
        box(b, {-half_w + 0.03f, 0.12f, door_z - 0.03f}, {half_w - 0.03f, body_h - 0.04f, door_z}, paint); // door
        box(b, {-half_w - 0.03f, body_h, -0.5f * depth - 0.03f}, {half_w + 0.03f, body_h + 0.05f, door_z + 0.03f}, shade(paint, 0.65f));
        for (const float sx : {-1.0f, 1.0f})
            for (const float sz : {-1.0f, 1.0f})
                box(b, {sx * (half_w - 0.08f) - 0.04f, 0.0f, sz * (0.5f * depth - 0.08f) - 0.04f},
                    {sx * (half_w - 0.08f) + 0.04f, 0.08f, sz * (0.5f * depth - 0.08f) + 0.04f}, dark_metal);
        // Handle, hinges and lower vents.
        box(b, {half_w - 0.1f, 0.9f, door_z}, {half_w - 0.07f, 1.5f, door_z + 0.04f}, {26, 26, 30, 255});
        for (const float y : {0.3f, 1.0f, 1.7f})
            column(b, {-half_w + 0.012f, y, door_z - 0.005f}, {-half_w + 0.012f, y + 0.12f, door_z - 0.005f}, 0.014f, 0.014f, 6, steel);
        for (int i = 0; i < 6; ++i)
        {
            const float y = 0.22f + 0.04f * static_cast<float>(i);
            box(b, {-0.22f, y, door_z}, {0.22f, y + 0.015f, door_z + 0.008f}, dark_metal);
        }
        // Hazard stripe and a data plate.
        for (int i = 0; i < 12; ++i)
            flat(b, -half_w + 0.075f + 0.05f * static_cast<float>(i), 0.165f, door_z + 0.0005f, 0.05f, 0.05f, 0.0f, 1.0f,
                 (i % 2 == 0) ? Colour{236, 196, 40, 255} : Colour{30, 30, 34, 255});
        column(b, {0.2f, body_h + 0.05f, 0.0f}, {0.2f, body_h + 0.2f, 0.0f}, 0.04f, 0.04f, 8, dark_metal);
        column(b, {0.2f, body_h + 0.2f, 0.0f}, {-0.5f, body_h + 0.2f, 0.0f}, 0.03f, 0.03f, 8, dark_metal);
        (void)panel_height;
        return b.data;
    }

    // ----------------------------------------------------------------------------------------------- Rockets and spacecraft

    namespace
    {
        struct Rp
        {
            float r, y;
        };

        /**
         * A surface of revolution about Y. The profile runs from the bottom centre outward and up; each segment faces
         * to its right (outward when going up, down when going outward), so closed solids need no orientation fuss.
         */
        void revolve(Builder &b, const std::vector<Rp> &profile, int sides, const std::function<Colour(int)> &colour)
        {
            const auto point = [](const Rp &p, float angle) { return V{p.r * std::sin(angle), p.y, p.r * std::cos(angle)}; };
            for (std::size_t k = 0; k + 1 < profile.size(); ++k)
            {
                const Rp p0 = profile[k], p1 = profile[k + 1];
                const float dr = p1.r - p0.r, dy = p1.y - p0.y;
                const float length = std::hypot(dr, dy);
                if (length < 1e-6f)
                    continue;
                const float nr = dy / length, ny = -dr / length;
                const Colour face = colour(static_cast<int>(k));
                for (int i = 0; i < sides; ++i)
                {
                    const float a0 = 2.0f * pi * static_cast<float>(i) / static_cast<float>(sides);
                    const float a1 = 2.0f * pi * static_cast<float>(i + 1) / static_cast<float>(sides);
                    const float am = 0.5f * (a0 + a1);
                    const V centre = 0.25f * (point(p0, a0) + point(p0, a1) + point(p1, a0) + point(p1, a1));
                    const V normal{nr * std::sin(am), ny, nr * std::cos(am)};
                    b.quad(point(p0, a0), point(p0, a1), point(p1, a1), point(p1, a0), face, centre - normal);
                }
            }
        }

        void add_part(Builder &b, MeshData part, const glm::mat4 &matrix)
        {
            transform_mesh_data(part, matrix);
            b.append(part, {0.0f, 0.0f, 0.0f}, 0.0f, 1.0f);
        }

        glm::mat4 at(const V &position) { return glm::translate(glm::mat4(1.0f), position); }

        /** A rocket bell: the exit is on `base`, the throat `length` above it. */
        void bell(Builder &b, const V &base, float length, float exit_radius)
        {
            const float throat = 0.3f * exit_radius;
            const std::vector<Rp> profile{{throat * 0.8f, length * 0.97f}, {exit_radius * 0.5f, length * 0.45f}, {exit_radius * 0.93f, 0.0f},
                                          {exit_radius, 0.0f},             {exit_radius * 0.58f, length * 0.45f}, {throat, length},
                                          {0.0f, length}};
            Builder local;
            revolve(local, profile, 12,
                    [](int k) { return k < 2 || k == 5 ? dark_metal : (k == 2 ? Colour{150, 154, 162, 255} : Colour{112, 116, 124, 255}); });
            b.append(local.data, base, 0.0f, 1.0f);
        }

        /** A circular docking port: a collar and a dark hatch, pointing along `axis`. */
        void port(Builder &b, const V &centre, const V &axis, float radius, Colour collar)
        {
            column(b, centre - axis * (0.1f * radius), centre + axis * (0.4f * radius), radius, radius * 0.86f, 12, collar);
            column(b, centre + axis * (0.38f * radius), centre + axis * (0.43f * radius), radius * 0.62f, radius * 0.62f, 12, dark_metal);
        }

        /** A crinkled foil face: an nu x nv grid of quads with their own shades. */
        void foil_face(Builder &b, const V &origin, const V &du, const V &dv, int nu, int nv, Colour foil, std::uint32_t seed, const V &inside)
        {
            const V cu = du / static_cast<float>(nu), cv = dv / static_cast<float>(nv);
            for (int i = 0; i < nu; ++i)
            {
                for (int j = 0; j < nv; ++j)
                {
                    const V a = origin + cu * static_cast<float>(i) + cv * static_cast<float>(j);
                    b.quad(a, a + cu, a + cu + cv, a + cv, shade(foil, 0.82f + 0.36f * hash01(seed * 131u + static_cast<std::uint32_t>(i * 17 + j))), inside);
                }
            }
        }
    } // namespace

    MeshData make_rocket_stage_data(float radius, float length, Colour body, Colour accent, std::uint32_t seed)
    {
        Builder b;
        const Colour skin = vary(body, seed, 0.03f);
        const float band = std::min(0.1f * length, 0.6f * radius);
        revolve(b, {{0.0f, 0.0f}, {radius, 0.0f}, {radius, band}, {radius, length - band}, {radius, length}, {0.0f, length}}, 20,
                [&](int k) { return k == 0 ? shade(accent, 0.8f) : (k == 1 || k == 3) ? accent : (k == 4 ? shade(skin, 0.85f) : skin); });
        const int rings = static_cast<int>(length / (3.0f * radius));
        for (int i = 1; i <= rings; ++i)
        {
            const float y = length * static_cast<float>(i) / static_cast<float>(rings + 1);
            revolve(b, {{radius, y - 0.12f}, {radius * 1.012f, y - 0.12f}, {radius * 1.012f, y + 0.12f}, {radius, y + 0.12f}}, 20,
                    [&](int) { return shade(skin, 0.86f); });
        }
        const float w = 0.06f * radius;
        box(b, {-w, band, radius - 0.03f}, {w, length - band, radius + 0.1f * radius}, shade(skin, 0.7f));
        return b.data;
    }

    MeshData make_rocket_engines_data(float stage_radius, int engines, float length)
    {
        Builder b;
        const float bell_length = length * 0.85f;
        const float exit = engines <= 1 ? 0.5f * stage_radius : 0.27f * stage_radius;
        std::vector<V> spots;
        if (engines <= 1)
            spots.push_back({0.0f, 0.0f, 0.0f});
        else
        {
            if (engines >= 5)
                spots.push_back({0.0f, 0.0f, 0.0f});
            const int ring = engines >= 5 ? 4 : engines;
            for (int i = 0; i < ring; ++i)
            {
                const float a = pi * 0.25f + 2.0f * pi * static_cast<float>(i) / static_cast<float>(ring);
                spots.push_back({0.58f * stage_radius * std::sin(a), 0.0f, 0.58f * stage_radius * std::cos(a)});
            }
        }
        for (const V &spot : spots)
            bell(b, spot, bell_length, exit);
        column(b, {0.0f, bell_length, 0.0f}, {0.0f, length, 0.0f}, stage_radius * 0.97f, stage_radius * 0.97f, 20, dark_metal);
        return b.data;
    }

    MeshData make_rocket_adapter_data(float bottom_radius, float top_radius, float length, Colour body, Colour accent)
    {
        Builder b;
        const float band = std::min(0.15f * length, 0.5f);
        const float rb = bottom_radius + (top_radius - bottom_radius) * band / length;
        const float rt = bottom_radius + (top_radius - bottom_radius) * (length - band) / length;
        revolve(b, {{0.0f, 0.0f}, {bottom_radius, 0.0f}, {rb, band}, {rt, length - band}, {top_radius, length}, {0.0f, length}}, 20,
                [&](int k) { return k == 0 ? shade(accent, 0.8f) : (k == 1 || k == 3) ? accent : (k == 4 ? shade(body, 0.85f) : body); });
        return b.data;
    }

    MeshData make_rocket_fins_data(float radius, float span, float chord, int count, Colour colour)
    {
        Builder b;
        const float half = 0.04f * span + 0.1f;
        for (int i = 0; i < count; ++i)
        {
            const float a = pi * 0.25f + 2.0f * pi * static_cast<float>(i) / static_cast<float>(count);
            const V radial{std::sin(a), 0.0f, std::cos(a)};
            const V tangent{radial.z, 0.0f, -radial.x};
            const auto ring = [&](float side)
            {
                return Ring{radial * (radius * 0.98f) + tangent * side, radial * (radius + span) + tangent * side + V{0.0f, 0.1f * chord, 0.0f},
                            radial * (radius + span) + tangent * side + V{0.0f, 0.55f * chord, 0.0f},
                            radial * (radius * 0.98f) + tangent * side + V{0.0f, chord, 0.0f}};
            };
            b.prism(ring(-half), ring(half), colour);
        }
        return b.data;
    }

    MeshData make_payload_fairing_data(float radius, float length, Colour body, Colour accent)
    {
        Builder b;
        const float cylinder = 0.4f * length;
        std::vector<Rp> profile{{0.0f, 0.0f}, {radius, 0.0f}, {radius, cylinder}};
        constexpr int steps = 6;
        for (int i = 1; i < steps; ++i)
        {
            const float t = static_cast<float>(i) / static_cast<float>(steps);
            profile.push_back({radius * std::pow(std::cos(t * 0.5f * pi), 0.75f), cylinder + t * (length - cylinder)});
        }
        profile.push_back({0.0f, length});
        const int tip = static_cast<int>(profile.size()) - 2;
        revolve(b, profile, 16, [&](int k) { return k == 0 ? shade(accent, 0.8f) : (k == tip ? accent : (k == 1 ? body : shade(body, 1.04f))); });
        return b.data;
    }

    MeshData make_bomb_data(float radius, float length, Colour casing, Colour band)
    {
        Builder b;
        const float tail = 0.28f * length, nose = 0.24f * length, body = length - tail - nose;
        std::vector<Rp> profile{{0.0f, 0.0f}, {radius * 0.45f, 0.0f}, {radius, tail}, {radius, tail + body}};
        constexpr int steps = 5;
        for (int i = 1; i < steps; ++i)
        {
            const float t = static_cast<float>(i) / static_cast<float>(steps);
            profile.push_back({radius * std::pow(std::cos(t * 0.5f * pi), 0.8f), tail + body + t * nose});
        }
        profile.push_back({0.0f, length});
        revolve(b, profile, 14, [&](int k) { return k == 1 ? shade(casing, 0.9f) : (k == 3 ? band : casing); });
        const float span = radius * 1.9f, chord = tail * 1.05f, half = 0.012f * length;
        for (int i = 0; i < 4; ++i)
        {
            const float a = 0.25f * pi + 0.5f * pi * static_cast<float>(i);
            const V radial{std::sin(a), 0.0f, std::cos(a)};
            const V tangent{radial.z, 0.0f, -radial.x};
            const auto ring = [&](float side)
            {
                return Ring{radial * (radius * 0.4f) + tangent * side, radial * span + tangent * side + V{0.0f, 0.1f * chord, 0.0f},
                            radial * span + tangent * side + V{0.0f, 0.7f * chord, 0.0f}, radial * (radius * 0.9f) + tangent * side + V{0.0f, chord, 0.0f}};
            };
            b.prism(ring(-half), ring(half), shade(casing, 0.8f));
        }
        column(b, {0.0f, 0.5f * chord, 0.0f}, {0.0f, 0.5f * chord + 0.02f, 0.0f}, span * 0.7f, span * 0.7f, 12, shade(casing, 0.7f), false, false);
        return b.data;
    }

    MeshData make_fatman_data(float radius, float length, Colour casing)
    {
        Builder b;
        const float tail = 0.3f * length, egg = length - tail;
        std::vector<Rp> profile{{0.0f, tail}};
        constexpr int steps = 10;
        for (int i = 0; i <= steps; ++i)
        {
            const float t = static_cast<float>(i) / static_cast<float>(steps);
            const float y = tail + t * egg;
            const float u = 2.0f * t - 1.0f;
            const float r = radius * std::sqrt(std::max(0.0f, 1.0f - u * u)) * (1.0f - 0.12f * u);
            profile.push_back({std::max(r, 0.001f), y});
        }
        profile.front() = {0.0f, tail};
        profile.back() = {0.0f, length};
        revolve(b, profile, 18, [&](int k) { return k % 3 == 0 ? shade(casing, 1.12f) : casing; });
        const float bw = radius * 0.5f;
        box(b, {-bw, 0.0f, -bw}, {bw, tail * 1.02f, bw}, shade(casing, 0.85f));
        for (int i = 0; i < 4; ++i)
        {
            const float a = 0.25f * pi + 0.5f * pi * static_cast<float>(i);
            const V radial{std::sin(a), 0.0f, std::cos(a)};
            const V tangent{radial.z, 0.0f, -radial.x};
            const float span = radius * 1.25f, half = 0.01f * length;
            const auto ring = [&](float side)
            {
                return Ring{radial * (bw * 0.7f) + tangent * side, radial * span + tangent * side, radial * span + tangent * side + V{0.0f, 0.6f * tail, 0.0f},
                            radial * (bw * 0.7f) + tangent * side + V{0.0f, tail, 0.0f}};
            };
            b.prism(ring(-half), ring(half), shade(casing, 0.75f));
        }
        return b.data;
    }

    MeshData make_launch_escape_tower_data(float height, Colour motor)
    {
        Builder b;
        const float base = 0.1f * height, top = 0.035f * height, leg = 0.008f * height;
        const float fractions[4] = {0.0f, 0.13f, 0.27f, 0.42f};
        std::vector<std::vector<V>> level(4);
        for (std::size_t k = 0; k < 4; ++k)
        {
            const float radius = base + (top - base) * fractions[k] / 0.42f;
            for (int i = 0; i < 4; ++i)
            {
                const float a = pi * 0.25f + 0.5f * pi * static_cast<float>(i);
                level[k].push_back({radius * std::sin(a), fractions[k] * height, radius * std::cos(a)});
            }
        }
        for (std::size_t i = 0; i < 4; ++i)
        {
            const std::size_t n = (i + 1) % 4;
            column(b, level[0][i], level[3][i], leg, leg, 4, steel);
            for (std::size_t k = 1; k < 4; ++k)
                column(b, level[k][i], level[k][n], leg * 0.7f, leg * 0.7f, 4, steel);
            for (std::size_t k = 0; k < 3; ++k)
                column(b, level[k][i], level[k + 1][n], leg * 0.6f, leg * 0.6f, 4, steel);
        }
        const float r = 0.035f * height;
        column(b, {0.0f, 0.4f * height, 0.0f}, {0.0f, 0.84f * height, 0.0f}, r * 1.1f, r, 12, motor);
        column(b, {0.0f, 0.4f * height, 0.0f}, {0.0f, 0.43f * height, 0.0f}, r * 1.3f, r * 1.3f, 12, dark_metal);
        for (int i = 0; i < 4; ++i)
        {
            const float a = 0.5f * pi * static_cast<float>(i);
            const V dir{std::sin(a), 0.0f, std::cos(a)};
            column(b, dir * (r * 0.9f) + V{0.0f, 0.5f * height, 0.0f}, dir * (r * 2.0f) + V{0.0f, 0.46f * height, 0.0f}, r * 0.35f, r * 0.2f, 6, dark_metal);
        }
        column(b, {0.0f, 0.84f * height, 0.0f}, {0.0f, 0.97f * height, 0.0f}, r, 0.0f, 12, {196, 70, 52, 255});
        column(b, {0.0f, 0.97f * height, 0.0f}, {0.0f, height, 0.0f}, 0.012f * height, 0.004f * height, 6, steel);
        return b.data;
    }

    MeshData make_antenna_dish_data(float radius)
    {
        Builder b;
        const float hub = 1.1f * radius;
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, hub, 0.0f}, 0.05f * radius, 0.04f * radius, 6, steel);
        Builder dish;
        const float depth = 0.28f * radius;
        std::vector<Rp> profile{{0.0f, 0.0f}};
        std::vector<Rp> inner;
        for (int i = 1; i <= 4; ++i)
        {
            const float t = static_cast<float>(i) / 4.0f;
            profile.push_back({radius * t, depth * t * t});
            inner.push_back({radius * t * 0.97f, depth * t * t + 0.02f * radius});
        }
        profile.push_back(inner[3]);
        for (int i = 2; i >= 0; --i)
            profile.push_back(inner[static_cast<std::size_t>(i)]);
        profile.push_back({0.0f, 0.02f * radius});
        revolve(dish, profile, 14, [](int k) { return k < 4 ? Colour{150, 152, 158, 255} : Colour{232, 232, 228, 255}; });
        column(dish, {0.0f, 0.02f * radius, 0.0f}, {0.0f, 0.6f * radius, 0.0f}, 0.04f * radius, 0.04f * radius, 6, steel);
        column(dish, {0.0f, 0.6f * radius, 0.0f}, {0.0f, 0.75f * radius, 0.0f}, 0.1f * radius, 0.06f * radius, 8, dark_metal);
        add_part(b, dish.data, at({0.0f, hub, 0.0f}) * glm::rotate(glm::mat4(1.0f), 70.0f * pi / 180.0f, V{1.0f, 0.0f, 0.0f}));
        return b.data;
    }

    MeshData make_apollo_csm_data(float radius, Colour hull)
    {
        Builder b;
        const float s = radius / 1.95f;
        const float sm0 = 2.2f * s, sm1 = 9.6f * s, cm1 = 12.8f * s;
        bell(b, {0.0f, 0.0f, 0.0f}, 2.6f * s, 1.0f * s);
        revolve(b, {{0.0f, sm0}, {radius, sm0}, {radius, sm1}, {0.0f, sm1}}, 16, [&](int k) { return k == 1 ? hull : shade(hull, 0.7f); });
        for (int i = 0; i < 8; ++i)
        {
            const float a = pi * 0.125f + 0.25f * pi * static_cast<float>(i);
            const V n{std::sin(a), 0.0f, std::cos(a)};
            panel(b, n * radius * 0.96f + V{0.0f, 0.5f * (sm0 + sm1) - 0.5f * s, 0.0f}, n, 1.3f * s, 4.6f * s,
                  i % 2 == 0 ? Colour{60, 62, 70, 255} : Colour{180, 184, 190, 255});
        }
        for (int i = 0; i < 4; ++i)
        {
            const float a = 0.5f * pi * static_cast<float>(i);
            const V n{std::sin(a), 0.0f, std::cos(a)};
            const V t{n.z, 0.0f, -n.x};
            const V c = n * (radius + 0.1f * s) + V{0.0f, sm1 - 1.0f * s, 0.0f};
            b.prism({c - t * 0.3f * s - V{0, 0.4f * s, 0}, c + t * 0.3f * s - V{0, 0.4f * s, 0}, c + t * 0.3f * s + V{0, 0.4f * s, 0}, c - t * 0.3f * s + V{0, 0.4f * s, 0}},
                    {c - t * 0.3f * s - V{0, 0.4f * s, 0} - n * 0.3f * s, c + t * 0.3f * s - V{0, 0.4f * s, 0} - n * 0.3f * s,
                     c + t * 0.3f * s + V{0, 0.4f * s, 0} - n * 0.3f * s, c - t * 0.3f * s + V{0, 0.4f * s, 0} - n * 0.3f * s},
                    dark_metal);
        }
        revolve(b, {{0.0f, sm1}, {radius * 1.02f, sm1}, {radius * 1.02f, sm1 + 0.2f * s}, {radius, sm1 + 0.2f * s}, {0.45f * s, cm1}, {0.0f, cm1}}, 16,
                [&](int k) { return k < 3 ? dark_metal : (k == 3 ? shade(hull, 1.08f) : steel); });
        const float slope = (radius - 0.45f * s) / (cm1 - sm1 - 0.2f * s);
        for (const float a : {-0.5f, 0.5f})
        {
            const V n = glm::normalize(V{std::sin(a), slope, std::cos(a)});
            panel(b, V{std::sin(a) * radius * 0.72f, sm1 + 1.3f * s, std::cos(a) * radius * 0.72f}, n, 0.5f * s, 0.6f * s, glass_dark);
        }
        column(b, {0.0f, cm1, 0.0f}, {0.0f, cm1 + 0.9f * s, 0.0f}, 0.4f * s, 0.12f * s, 8, steel);
        add_part(b, make_antenna_dish_data(0.55f * s), at({radius + 0.2f * s, sm0 + 2.0f * s, 0.0f}) * glm::rotate(glm::mat4(1.0f), -0.5f * pi, V{0.0f, 0.0f, 1.0f}));
        return b.data;
    }

    MeshData make_saturn_v_data(float scale)
    {
        Builder b;
        const Colour white{238, 238, 232, 255}, black{26, 26, 30, 255};
        float y = 3.0f;
        const auto stack = [&](MeshData part, float height)
        {
            add_part(b, std::move(part), at({0.0f, y, 0.0f}));
            y += height;
        };
        add_part(b, make_rocket_engines_data(5.05f, 5, 5.9f), at({0.0f, 0.0f, 0.0f}));
        add_part(b, make_rocket_fins_data(5.05f, 3.2f, 8.0f, 4, white), at({0.0f, 0.5f, 0.0f}));
        stack(make_rocket_stage_data(5.05f, 39.1f, white, black, 1), 39.1f);
        stack(make_rocket_stage_data(5.05f, 5.1f, white, black, 2), 5.1f);
        stack(make_rocket_stage_data(5.05f, 24.9f, white, black, 3), 24.9f);
        stack(make_rocket_adapter_data(5.05f, 3.3f, 3.3f, white, black), 3.3f);
        stack(make_rocket_stage_data(3.3f, 17.8f, white, black, 4), 17.8f);
        stack(make_rocket_stage_data(3.3f, 0.9f, white, black, 5), 0.9f);
        stack(make_rocket_adapter_data(3.3f, 1.95f, 5.5f, white, black), 5.5f);
        stack(make_rocket_adapter_data(1.95f, 0.45f, 3.2f, white, black), 3.2f);
        stack(make_launch_escape_tower_data(10.0f), 10.0f);
        MeshData out = b.data;
        if (scale != 1.0f)
            transform_mesh_data(out, glm::scale(glm::mat4(1.0f), V{scale}));
        return out;
    }

    MeshData make_launch_tower_data(float height, float width, float arm_length, Colour colour)
    {
        Builder b;
        const float h = 0.5f * width, post = 0.06f * width;
        const int levels = std::max(2, static_cast<int>(height / std::max(4.0f, height / 18.0f)));
        const float step = height / static_cast<float>(levels);
        const V corners[4] = {{-h, 0.0f, -h}, {h, 0.0f, -h}, {h, 0.0f, h}, {-h, 0.0f, h}};
        for (const V &c : corners)
            box(b, c + V{-post, 0.0f, -post}, c + V{post, height, post}, colour);
        const float beam = 0.025f * width, brace = 0.018f * width;
        for (int k = 0; k <= levels; ++k)
        {
            const float y = step * static_cast<float>(k);
            for (int i = 0; i < 4; ++i)
            {
                const V a = corners[i] + V{0.0f, y, 0.0f}, c = corners[(i + 1) % 4] + V{0.0f, y, 0.0f};
                column(b, a, c, beam, beam, 4, shade(colour, 0.85f), false, false);
                if (k < levels)
                {
                    const bool flip = (k + i) % 2 == 0;
                    column(b, flip ? a : c, (flip ? c : a) + V{0.0f, step, 0.0f}, brace, brace, 4, shade(colour, 0.8f), false, false);
                }
            }
        }
        for (int k = 1; k <= 3; ++k)
        {
            const float y = height * 0.25f * static_cast<float>(k) - 0.5f * step;
            box(b, {h, y - 0.8f, -0.9f}, {h + arm_length, y + 0.8f, 0.9f}, {210, 210, 205, 255});
            box(b, {h + arm_length - 1.2f, y - 1.2f, -1.3f}, {h + arm_length, y + 1.2f, 1.3f}, dark_metal);
        }
        box(b, {-h - 3.0f, height, -1.0f}, {h + 1.0f, height + 1.6f, 1.0f}, shade(colour, 0.9f));
        column(b, {0.0f, height + 1.6f, 0.0f}, {0.0f, height + 7.0f, 0.0f}, 0.15f, 0.05f, 6, steel);
        return b.data;
    }

    MeshData make_launch_pad_data(float size)
    {
        Builder b;
        const float half = 0.5f * size, hole = 0.16f * size, thick = 1.5f;
        const Colour concrete{150, 148, 142, 255};
        box(b, {-half, 0.0f, -half}, {half, thick, -hole}, concrete);
        box(b, {-half, 0.0f, hole}, {half, thick, half}, concrete);
        box(b, {-half, 0.0f, -hole}, {-hole, thick, hole}, shade(concrete, 0.95f));
        box(b, {hole, 0.0f, -hole}, {half, thick, hole}, shade(concrete, 0.95f));
        box(b, {-hole, 0.0f, -hole}, {hole, thick + 0.02f, hole}, {40, 36, 34, 255}); // flame-deflector plate, level with the deck so ships can rest on it
        for (const float x : {-1.0f, 1.0f})
        {
            for (const float z : {-1.0f, 1.0f})
            {
                const V foot{x * 0.42f * size, thick, z * 0.42f * size};
                column(b, foot, foot + V{0.0f, 0.4f * size, 0.0f}, 0.25f, 0.12f, 6, steel);
                column(b, foot + V{0.0f, 0.4f * size, 0.0f}, foot + V{0.0f, 0.4f * size + 1.5f, 0.0f}, 0.12f, 0.0f, 6, {230, 60, 50, 255});
            }
        }
        return b.data;
    }

    MeshData make_station_module_data(float radius, float length, Colour hull, std::uint32_t seed)
    {
        Builder b;
        const Colour skin = vary(hull, seed, 0.03f);
        const int sections = std::max(2, static_cast<int>(length / (1.4f * radius)));
        const auto section_y = [&](int i) { return (0.03f + 0.94f * static_cast<float>(i) / static_cast<float>(sections)) * length; };
        std::vector<Rp> profile{{0.0f, 0.0f}, {radius * 0.86f, 0.0f}};
        for (int i = 0; i < sections; ++i)
            profile.push_back({radius, section_y(i)});
        profile.push_back({radius, 0.97f * length});
        profile.push_back({radius * 0.86f, length});
        profile.push_back({0.0f, length});
        const int last = static_cast<int>(profile.size()) - 2;
        revolve(b, profile, 16, [&](int k) { return (k == 0 || k == last) ? dark_metal : ((k % 2 == 0) ? shade(skin, 0.93f) : skin); });
        for (int i = 1; i < sections; ++i)
        {
            const float y = section_y(i);
            revolve(b, {{radius, y - 0.06f}, {radius * 1.01f, y - 0.06f}, {radius * 1.01f, y + 0.06f}, {radius, y + 0.06f}}, 16, [](int) { return dark_metal; });
        }
        for (const float a : {-0.45f, 0.0f, 0.45f})
            panel(b, {std::sin(a) * radius, 0.62f * length, std::cos(a) * radius}, {std::sin(a), 0.0f, std::cos(a)}, 0.35f * radius, 0.45f * radius, glass_dark);
        port(b, {0.0f, length, 0.0f}, {0.0f, 1.0f, 0.0f}, 0.28f * radius, steel);
        return b.data;
    }

    MeshData make_docking_adapter_data(float radius, float length, Colour hull)
    {
        Builder b;
        revolve(b, {{0.0f, 0.0f}, {radius, 0.0f}, {radius, 0.8f * length}, {0.55f * radius, length}, {0.0f, length}}, 14,
                [&](int k) { return k == 0 ? dark_metal : (k == 1 ? hull : shade(hull, 1.06f)); });
        port(b, {0.0f, length, 0.0f}, {0.0f, 1.0f, 0.0f}, 0.4f * radius, steel);
        port(b, {0.0f, 0.4f * length, radius}, {0.0f, 0.0f, 1.0f}, 0.4f * radius, steel);
        return b.data;
    }

    MeshData make_solar_array_data(float length, float width, int panels, Colour cell)
    {
        Builder b;
        panels = std::max(1, panels);
        const float pw = length / static_cast<float>(panels), t = 0.03f, hw = 0.5f * width;
        const Colour frame{170, 172, 178, 255};
        box(b, {0.0f, -0.05f, -0.06f}, {length, 0.05f, 0.06f}, dark_metal);
        const int cols = 3, rows = std::max(3, static_cast<int>(width / 0.7f));
        for (int p = 0; p < panels; ++p)
        {
            const float x0 = pw * static_cast<float>(p) + 0.03f * pw, x1 = pw * static_cast<float>(p + 1) - 0.03f * pw;
            box(b, {x0, -hw, -t}, {x1, hw, t}, frame);
            const float inset = 0.05f * std::min(pw, width);
            const float cw = (x1 - x0 - 2.0f * inset) / static_cast<float>(cols), ch = (width - 2.0f * inset) / static_cast<float>(rows);
            for (const float side : {1.0f, -1.0f})
            {
                const float z = side * (t + 0.004f);
                for (int i = 0; i < cols; ++i)
                {
                    for (int j = 0; j < rows; ++j)
                    {
                        const float cx = x0 + inset + cw * static_cast<float>(i) + 0.012f, cy = -hw + inset + ch * static_cast<float>(j) + 0.012f;
                        const Colour c = shade(cell, 0.85f + 0.3f * hash01(static_cast<std::uint32_t>(p * 977 + i * 131 + j * 17 + (side > 0.0f ? 5 : 0))));
                        b.quad({cx, cy, z}, {cx + cw - 0.024f, cy, z}, {cx + cw - 0.024f, cy + ch - 0.024f, z}, {cx, cy + ch - 0.024f, z}, c, {cx, cy, 0.0f});
                    }
                }
            }
        }
        return b.data;
    }

    MeshData make_solar_windmill_data(float blade_length, float blade_width)
    {
        Builder b;
        box(b, {-0.6f, -0.6f, -0.5f}, {0.6f, 0.6f, 0.5f}, {196, 164, 80, 255});
        for (int i = 0; i < 4; ++i)
            add_part(b, make_solar_array_data(blade_length, blade_width, 2),
                     glm::rotate(glm::mat4(1.0f), 0.5f * pi * static_cast<float>(i), V{0.0f, 0.0f, 1.0f}) * at({0.55f, 0.0f, 0.0f}));
        return b.data;
    }

    MeshData make_satellite_bus_data(float size, Colour foil, std::uint32_t seed)
    {
        Builder b;
        const float w = 0.5f * size, h = 1.1f * size, d = 0.45f * size, y0 = 0.15f * size;
        const V c{0.0f, y0 + 0.5f * h, 0.0f};
        column(b, {0.0f, 0.0f, 0.0f}, {0.0f, y0, 0.0f}, 0.4f * size, 0.34f * size, 12, steel);
        const V lo{-w, y0, -d}, hi{w, y0 + h, d};
        foil_face(b, {lo.x, lo.y, hi.z}, {2 * w, 0, 0}, {0, h, 0}, 3, 4, foil, seed, c);
        foil_face(b, lo, {2 * w, 0, 0}, {0, h, 0}, 3, 4, foil, seed + 1, c);
        foil_face(b, {hi.x, lo.y, lo.z}, {0, 0, 2 * d}, {0, h, 0}, 3, 4, foil, seed + 2, c);
        foil_face(b, lo, {0, 0, 2 * d}, {0, h, 0}, 3, 4, foil, seed + 3, c);
        foil_face(b, {lo.x, hi.y, lo.z}, {2 * w, 0, 0}, {0, 0, 2 * d}, 3, 3, foil, seed + 4, c);
        foil_face(b, lo, {2 * w, 0, 0}, {0, 0, 2 * d}, 3, 3, shade(foil, 0.6f), seed + 5, c);
        const float lens_y = c.y + 0.1f * h;
        column(b, {0.0f, lens_y, d}, {0.0f, lens_y, d + 0.35f * size}, 0.17f * size, 0.2f * size, 10, dark_metal);
        column(b, {0.0f, lens_y, d + 0.34f * size}, {0.0f, lens_y, d + 0.36f * size}, 0.14f * size, 0.14f * size, 10, {70, 110, 160, 255});
        for (const float x : {-1.0f, 1.0f})
        {
            for (const float z : {-1.0f, 1.0f})
            {
                const V corner{x * w, hi.y, z * d};
                column(b, corner, corner + V{x * 0.1f * size, 0.12f * size, z * 0.1f * size}, 0.025f * size, 0.05f * size, 6, steel);
            }
        }
        column(b, {0.0f, hi.y, 0.0f}, {0.0f, hi.y + 0.7f * size, 0.0f}, 0.012f * size, 0.004f * size, 4, steel);
        box(b, {-0.12f * size, c.y - 0.2f * h, d}, {0.12f * size, c.y - 0.1f * h, d + 0.03f * size}, {196, 198, 204, 255});
        return b.data;
    }

    MeshData make_satellite_data(float size, std::uint32_t seed)
    {
        Builder b;
        b.append(make_satellite_bus_data(size, {200, 164, 72, 255}, seed), {0.0f, 0.0f, 0.0f}, 0.0f, 1.0f);
        const float mid = 0.15f * size + 0.55f * size;
        add_part(b, make_solar_array_data(2.2f * size, 0.9f * size, 3), at({0.5f * size, mid, 0.0f}));
        add_part(b, make_solar_array_data(2.2f * size, 0.9f * size, 3), at({-0.5f * size, mid, 0.0f}) * glm::rotate(glm::mat4(1.0f), pi, V{0.0f, 1.0f, 0.0f}));
        add_part(b, make_antenna_dish_data(0.45f * size), at({0.25f * size, 1.25f * size, 0.0f}));
        return b.data;
    }

    MeshData make_skylab_data(float scale)
    {
        Builder b;
        const Colour white{238, 238, 232, 255};
        float y = 0.0f;
        const auto stack = [&](MeshData part, float height)
        {
            add_part(b, std::move(part), at({0.0f, y, 0.0f}));
            y += height;
        };
        stack(make_station_module_data(3.3f, 14.6f, white, 1), 14.6f);
        stack(make_rocket_adapter_data(3.3f, 1.5f, 2.4f, white, {60, 60, 66, 255}), 2.4f);
        stack(make_station_module_data(1.5f, 5.4f, {222, 222, 216, 255}, 2), 5.4f);
        const float mda = y;
        stack(make_docking_adapter_data(1.5f, 5.2f), 5.2f);
        const float wing_y = 7.3f;
        add_part(b, make_solar_array_data(9.0f, 11.0f, 2), at({3.3f, wing_y, 0.0f}));
        add_part(b, make_solar_array_data(9.0f, 11.0f, 2), at({-3.3f, wing_y, 0.0f}) * glm::rotate(glm::mat4(1.0f), pi, V{0.0f, 1.0f, 0.0f}));
        const float atm_y = mda + 2.6f;
        box(b, {1.4f, atm_y - 0.25f, -0.25f}, {7.0f, atm_y + 0.25f, 0.25f}, steel);
        add_part(b, make_station_module_data(1.2f, 5.0f, {206, 190, 120, 255}, 3), at({7.4f, atm_y - 2.5f, 0.0f}));
        add_part(b, make_solar_windmill_data(6.5f, 3.0f), at({7.4f, atm_y, 2.6f}));
        MeshData out = b.data;
        if (scale != 1.0f)
            transform_mesh_data(out, glm::scale(glm::mat4(1.0f), V{scale}));
        return out;
    }
} // namespace sl
