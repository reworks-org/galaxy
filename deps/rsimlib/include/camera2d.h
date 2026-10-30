#pragma once

#include "display.h"
#include "draw.h"

#include <algorithm>
#include <cmath>

namespace sl
{
    /**
     * A 2D camera: a view onto a world with pan, zoom, rotation, smooth following, world bounds and screen shake.
     *
     * The camera looks at (x, y) in world space, which appears at the centre of the viewport. Use world_to_screen() and
     * screen_to_world() for game logic and draw_sprite() to draw world-space sprites. Tilemaps can be drawn with
     * top_left() as the camera position (draw_tilemap supports panning and shake; zoom and rotation do not apply to it).
     */
    struct Camera2D
    {
        /** World position shown at the centre of the viewport. */
        float x = 0.0f, y = 0.0f;
        /** Magnification: 2 draws everything twice as large. */
        float zoom = 1.0f;
        /** Clockwise view rotation in degrees. The world appears to turn anticlockwise. */
        float rotation = 0.0f;
        /** Viewport size in pixels; 0 uses the screen size. */
        int viewport_width = 0, viewport_height = 0;
        /** Viewport position on screen (the centre of the view is offset by this). */
        float viewport_x = 0.0f, viewport_y = 0.0f;

        /** Constrain the camera so the view stays inside a world rectangle. */
        void set_bounds(float left, float top, float right, float bottom)
        {
            bounded_ = true;
            bounds_left_ = left;
            bounds_top_ = top;
            bounds_right_ = right;
            bounds_bottom_ = bottom;
            clamp_to_bounds();
        }
        /** Remove the world bounds. */
        void clear_bounds() { bounded_ = false; }

        /**
         * Move toward a world position. responsiveness is how quickly the camera closes the gap in 1/seconds
         * (higher is snappier); zero or less snaps immediately. Frame-rate independent.
         */
        void follow(float target_x, float target_y, float dt_seconds, float responsiveness = 0.0f)
        {
            if (responsiveness <= 0.0f || dt_seconds <= 0.0f)
            {
                x = target_x;
                y = target_y;
            }
            else
            {
                const float t = 1.0f - std::exp(-responsiveness * dt_seconds);
                x += (target_x - x) * t;
                y += (target_y - y) * t;
            }
            clamp_to_bounds();
        }

        /** Start a screen shake of up to magnitude pixels that fades out over duration seconds. */
        void shake(float magnitude, float duration_seconds)
        {
            shake_magnitude_ = std::max(magnitude, 0.0f);
            shake_duration_ = std::max(duration_seconds, 0.0f);
            shake_time_ = shake_duration_;
        }

        /** Advance the shake. Call once per frame. */
        void update(float dt_seconds)
        {
            if (shake_time_ <= 0.0f)
            {
                shake_offset_x_ = shake_offset_y_ = 0.0f;
                return;
            }
            shake_time_ = std::max(shake_time_ - dt_seconds, 0.0f);
            const float fade = shake_duration_ > 0.0f ? shake_time_ / shake_duration_ : 0.0f;
            shake_offset_x_ = next_random() * shake_magnitude_ * fade;
            shake_offset_y_ = next_random() * shake_magnitude_ * fade;
        }

        /** Return whether a shake is in progress. */
        bool shaking() const { return shake_time_ > 0.0f; }

        /** Convert a world position to a screen position. */
        void world_to_screen(float world_x, float world_y, float &screen_x, float &screen_y) const
        {
            const float dx = world_x - x;
            const float dy = world_y - y;
            const float radians = -rotation * 3.14159265358979f / 180.0f;
            const float c = std::cos(radians);
            const float s = std::sin(radians);
            screen_x = viewport_x + view_width() * 0.5f + shake_offset_x_ + zoom * (dx * c - dy * s);
            screen_y = viewport_y + view_height() * 0.5f + shake_offset_y_ + zoom * (dx * s + dy * c);
        }

        /** Convert a screen position to a world position. */
        void screen_to_world(float screen_x, float screen_y, float &world_x, float &world_y) const
        {
            const float safe_zoom = zoom != 0.0f ? zoom : 1.0f;
            const float dx = (screen_x - viewport_x - view_width() * 0.5f - shake_offset_x_) / safe_zoom;
            const float dy = (screen_y - viewport_y - view_height() * 0.5f - shake_offset_y_) / safe_zoom;
            const float radians = rotation * 3.14159265358979f / 180.0f;
            const float c = std::cos(radians);
            const float s = std::sin(radians);
            world_x = x + dx * c - dy * s;
            world_y = y + dx * s + dy * c;
        }

        /** The world-space axis-aligned rectangle that covers the view, including any rotation. */
        void visible_rect(float &left, float &top, float &right, float &bottom) const
        {
            const float corners[4][2] = {{viewport_x, viewport_y},
                                         {viewport_x + view_width(), viewport_y},
                                         {viewport_x + view_width(), viewport_y + view_height()},
                                         {viewport_x, viewport_y + view_height()}};
            left = top = 1e30f;
            right = bottom = -1e30f;
            for (const auto &corner : corners)
            {
                float wx = 0.0f, wy = 0.0f;
                screen_to_world(corner[0], corner[1], wx, wy);
                left = std::min(left, wx);
                right = std::max(right, wx);
                top = std::min(top, wy);
                bottom = std::max(bottom, wy);
            }
        }

        /** World position of the viewport's top-left corner, for draw_tilemap() (ignores zoom and rotation). */
        void top_left(float &world_x, float &world_y) const
        {
            world_x = x - view_width() * 0.5f - shake_offset_x_;
            world_y = y - view_height() * 0.5f - shake_offset_y_;
        }

        /**
         * Draw a sprite whose position, scale and rotation are in world space. Only the position, scale and
         * rotation in the parameters are transformed; origin, flips, source rectangle and tint are unchanged.
         */
        void draw_sprite(Bitmap *bitmap, SpriteParams params) const
        {
            world_to_screen(params.x, params.y, params.x, params.y);
            params.scale_x *= zoom;
            params.scale_y *= zoom;
            params.rotation -= rotation;
            draw_sprite_ex(bitmap, params);
        }

        /** Width of the viewport in pixels. */
        float view_width() const { return static_cast<float>(viewport_width > 0 ? viewport_width : screen_width()); }
        /** Height of the viewport in pixels. */
        float view_height() const { return static_cast<float>(viewport_height > 0 ? viewport_height : screen_height()); }

    private:
        void clamp_to_bounds()
        {
            if (!bounded_ || zoom == 0.0f)
                return;
            const float half_width = view_width() * 0.5f / std::abs(zoom);
            const float half_height = view_height() * 0.5f / std::abs(zoom);
            const float min_x = bounds_left_ + half_width;
            const float max_x = bounds_right_ - half_width;
            const float min_y = bounds_top_ + half_height;
            const float max_y = bounds_bottom_ - half_height;
            x = min_x > max_x ? (bounds_left_ + bounds_right_) * 0.5f : std::clamp(x, min_x, max_x);
            y = min_y > max_y ? (bounds_top_ + bounds_bottom_) * 0.5f : std::clamp(y, min_y, max_y);
        }

        // Small deterministic generator returning a value in [-1, 1].
        float next_random()
        {
            seed_ = seed_ * 1664525u + 1013904223u;
            return static_cast<float>((seed_ >> 8) & 0xFFFF) / 32767.5f - 1.0f;
        }

        bool bounded_ = false;
        float bounds_left_ = 0.0f, bounds_top_ = 0.0f, bounds_right_ = 0.0f, bounds_bottom_ = 0.0f;
        float shake_magnitude_ = 0.0f, shake_duration_ = 0.0f, shake_time_ = 0.0f;
        float shake_offset_x_ = 0.0f, shake_offset_y_ = 0.0f;
        unsigned int seed_ = 12345u;
    };
} // namespace sl
