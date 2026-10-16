#pragma once

#include <algorithm>
#include <cmath>

namespace sl
{
    /**
     * @brief Configuration parameters for isometric projection.
     *
     * Standard dimetric 2:1 isometric tiles have tile_height = tile_width / 2.
     */
    struct IsometricConfig
    {
        /** Full width of one tile in screen pixels at zoom 1. */
        float tile_width = 64.0f;
        /** Full height of one tile in screen pixels at zoom 1. */
        float tile_height = 32.0f;
        /** Screen pixels per world elevation unit at zoom 1. */
        float elevation_height = 16.0f;
    };

    /**
     * @brief 2D point representing screen or continuous world coordinates.
     */
    struct IsoPoint
    {
        /** Horizontal coordinate. */
        float x = 0.0f;
        /** Vertical coordinate. */
        float y = 0.0f;
    };

    /**
     * @brief Discrete grid cell coordinates in isometric tilemap space.
     */
    struct IsoGridPoint
    {
        /** Tile column. */
        int x = 0;
        /** Tile row. */
        int y = 0;
    };

    /**
     * @brief Axis-aligned tile coordinate bounding box in isometric grid space.
     */
    struct IsoBounds
    {
        /** Minimum tile column, inclusive. */
        int min_x = 0;
        /** Minimum tile row, inclusive. */
        int min_y = 0;
        /** Maximum tile column, inclusive. */
        int max_x = 0;
        /** Maximum tile row, inclusive. */
        int max_y = 0;
    };

    /**
     * @brief The four vertices of an isometric tile diamond in screen space.
     */
    struct IsoTileDiamond
    {
        /** Top vertex in screen coordinates. */
        IsoPoint top;
        /** Right vertex in screen coordinates. */
        IsoPoint right;
        /** Bottom vertex in screen coordinates. */
        IsoPoint bottom;
        /** Left vertex in screen coordinates. */
        IsoPoint left;
    };

    /**
     * @brief Pure mathematical transformation layer between 3D isometric world space (x, y, z)
     *        and 2D screen space (sx, sy).
     *
     * In world coordinates:
     * - +X points down-right along the isometric grid.
     * - +Y points down-left along the isometric grid.
     * - +Z points upward (elevation/height).
     */
    struct IsometricTransform
    {
        /** Tile geometry and elevation scale. */
        IsometricConfig config;
        /** Screen-space horizontal origin in pixels. */
        float origin_x = 0.0f;
        /** Screen-space vertical origin in pixels. */
        float origin_y = 0.0f;
        /** Uniform zoom multiplier applied to tile and elevation dimensions. */
        float zoom = 1.0f;

        /**
         * @brief Convert 3D isometric world coordinates (x, y, z) to 2D screen coordinates (sx, sy).
         */
        void world_to_screen(float x, float y, float z, float &sx, float &sy) const
        {
            const float half_w = (config.tile_width * 0.5f) * zoom;
            const float half_h = (config.tile_height * 0.5f) * zoom;
            const float elev = (config.elevation_height * zoom) * z;

            sx = (x - y) * half_w + origin_x;
            sy = (x + y) * half_h - elev + origin_y;
        }

        /**
         * @brief Convert 3D isometric world coordinates (x, y, z) to 2D screen coordinates.
         */
        IsoPoint world_to_screen(float x, float y, float z = 0.0f) const
        {
            IsoPoint screen;
            world_to_screen(x, y, z, screen.x, screen.y);
            return screen;
        }

        /**
         * @brief Convert 2D screen coordinates (sx, sy) on elevation plane z to continuous world coordinates (x, y).
         */
        void screen_to_world(float sx, float sy, float z, float &x, float &y) const
        {
            const float half_w = (config.tile_width * 0.5f) * zoom;
            const float half_h = (config.tile_height * 0.5f) * zoom;
            const float elev = (config.elevation_height * zoom) * z;

            const float adj_x = sx - origin_x;
            const float adj_y = sy - origin_y + elev;

            x = (adj_y / half_h + adj_x / half_w) * 0.5f;
            y = (adj_y / half_h - adj_x / half_w) * 0.5f;
        }

        /**
         * @brief Convert 2D screen coordinates (sx, sy) on elevation plane z to continuous world coordinates.
         */
        IsoPoint screen_to_world(float sx, float sy, float z = 0.0f) const
        {
            IsoPoint world;
            screen_to_world(sx, sy, z, world.x, world.y);
            return world;
        }

        /**
         * @brief Convert 2D screen coordinates (sx, sy) on elevation plane z to discrete grid cell coordinates.
         */
        IsoGridPoint screen_to_grid(float sx, float sy, float z = 0.0f) const
        {
            float wx = 0.0f;
            float wy = 0.0f;
            screen_to_world(sx, sy, z, wx, wy);
            return {
                static_cast<int>(std::floor(wx)),
                static_cast<int>(std::floor(wy))
            };
        }

        /**
         * @brief Compute the 4 screen-space diamond vertices for a tile at grid position (x, y, z).
         */
        IsoTileDiamond tile_diamond(float x, float y, float z = 0.0f) const
        {
            const float half_w = (config.tile_width * 0.5f) * zoom;
            const float half_h = (config.tile_height * 0.5f) * zoom;

            float cx = 0.0f;
            float cy = 0.0f;
            // Center of tile is (x + 0.5, y + 0.5)
            world_to_screen(x + 0.5f, y + 0.5f, z, cx, cy);

            return {
                {cx, cy - half_h}, // Top
                {cx + half_w, cy}, // Right
                {cx, cy + half_h}, // Bottom
                {cx - half_w, cy}  // Left
            };
        }

        /**
         * @brief Compute visible grid bounds for a 2D screen rectangle [left, top, right, bottom].
         *
         * @param left Viewport left coordinate (e.g. 0).
         * @param top Viewport top coordinate (e.g. 0).
         * @param right Viewport right coordinate (e.g. screen_width()).
         * @param bottom Viewport bottom coordinate (e.g. screen_height()).
         * @param min_z Minimum elevation plane to consider for culling.
         * @param max_z Maximum elevation plane to consider for culling.
         * @return Bounding box of all potentially visible grid tiles.
         */
        IsoBounds visible_bounds(float left, float top, float right, float bottom,
                                float min_z = 0.0f, float max_z = 0.0f) const
        {
            float min_gx = 1e9f;
            float min_gy = 1e9f;
            float max_gx = -1e9f;
            float max_gy = -1e9f;

            const float corners[4][2] = {
                {left, top},
                {right, top},
                {right, bottom},
                {left, bottom}
            };

            const float z_planes[2] = {min_z, max_z};
            for (float z : z_planes)
            {
                for (const auto &corner : corners)
                {
                    float gx = 0.0f;
                    float gy = 0.0f;
                    screen_to_world(corner[0], corner[1], z, gx, gy);

                    min_gx = std::min(min_gx, gx);
                    min_gy = std::min(min_gy, gy);
                    max_gx = std::max(max_gx, gx);
                    max_gy = std::max(max_gy, gy);
                }
            }

            return {
                static_cast<int>(std::floor(min_gx)) - 1,
                static_cast<int>(std::floor(min_gy)) - 1,
                static_cast<int>(std::ceil(max_gx)) + 1,
                static_cast<int>(std::ceil(max_gy)) + 1
            };
        }

        /**
         * @brief Calculate depth sorting key for back-to-front rendering order.
         *
         * (X + Y) must dominate the key so that a tile which is diagonally further
         * forward always renders after (on top of) every tile diagonally behind it,
         * no matter how tall either column is. Z is only used to order layers within
         * the same (x, y) column, and must never be large enough to outweigh a
         * one-unit change in X or Y (which would let a tall column behind erroneously
         * paint over a shorter column in front, hiding its top face).
         */
        static float depth_sort_key(float x, float y, float z = 0.0f)
        {
            return (x + y) * 64.0f + z;
        }
    };

} // namespace sl
