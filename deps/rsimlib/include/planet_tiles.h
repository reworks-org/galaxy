#pragma once

#include "async_load.h"
#include "draw.h"
#include "surface3d.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace sl
{

    /** Settings for PlanetTiles::create(). */
    struct PlanetTilesOptions
    {
        /**
         * The colour map: a Celestia virtual texture (.ctx file) or the folder holding its `levelN` folders. Each level
         * folder holds `tx_<column>_<row>.<ext>` tiles (dds, jpg or png). Level 0 is 2 x 1 tiles (more with BaseSplit),
         * every level doubles both directions, column 0 starts at longitude -180 and row 0 is the north.
         */
        std::string colour;
        /**
         * Optional gloss map in the same layout (white is shiny, as in Celestia's SpecularTexture), drawn as a
         * sun-glint overlay. May stop at a shallower level than the colour map.
         */
        std::string specular;
        /**
         * Optional tangent-space normal map in the same layout. A patch only uses it when the normal tile exists at the
         * same level as the colour tile it draws, so it fades out where the colour map is deeper.
         */
        std::string normal;
        /** Quads along each side of a tile's patch of sphere. */
        int segments = 12;
        /** Refine until one texel covers at most this many screen pixels (1 is full detail, 2 halves the resolution). */
        float texel_pixels = 1.0f;
        /** Deepest level to use; -1 for the deepest one present. */
        int max_level = -1;
        /** Most tile textures kept in memory (the coarsest level is never evicted). */
        int max_cached_tiles = 512;
        /** Most tile loads running at once; more are queued by the next update(). */
        int max_loads_in_flight = 6;
        /** Pixel width of a tile when the descriptor does not say (it is read from the first tile). 0 to detect. */
        int tile_size = 0;
        /** Texture filtering for the tiles. */
        float anisotropy = 4.0f;
        /** Brightness of the specular overlay, 0-1. */
        float specular_strength = 1.0f;
        /** Roughness of the shiniest places, so a white texel is not a mirror. 0.04-1. */
        float specular_min_roughness = 0.15f;
        /** How far, in tile widths, the skirt around each patch dips to hide cracks between levels of detail. */
        float skirt = 0.05f;
        /**
         * Where a regional high-resolution patch ends, the sharp tile fades out over this fraction of its width onto the
         * coarser tile beneath, instead of stopping at a hard edge. 0 turns it off.
         */
        float feather = 0.3f;
        /** Seconds a tile takes to fade in over its parent when it finishes loading, so detail does not pop. 0 turns it off. */
        float fade_seconds = 0.4f;
    };

    /** What PlanetTiles is doing, for overlays and tests. */
    struct PlanetTilesStats
    {
        int patches = 0;
        int deepest_level = 0;
        int tiles_loaded = 0;
        int tiles_loading = 0;
        int meshes = 0;
    };

    /**
     * A planet drawn from a pyramid of texture tiles that are streamed in as the camera approaches, so a 64k x 32k map
     * needs only the tiles that are in view.
     *
     * Each frame call update() with the camera, then draw(). The sphere is split into patches by a quadtree: a tile is
     * refined into its four children while its texels are bigger than the screen pixels, and its children are only
     * refined once the tile itself is resident. Tiles load on worker threads (see load_bitmap_async()); until a tile
     * arrives its patch draws the part of the next shallower tile that covers it. The pyramid may be sparse: where a
     * level has no tile (for example a regional high-resolution patch), the patch keeps using the coarser tile.
     *
     * The unit sphere has its poles on +-Y and longitude -180 at +X. East (+Z at longitude 0, which faces -X) is to the
     * right when seen from outside, which is the reverse of create_sphere_mesh()'s texture direction, so a mesh that
     * replaces a sphere needs its u coordinate mirrored. All of it belongs to the main thread.
     */
    class PlanetTiles
    {
    public:
        /** Open the tile sets and load the coarsest level. Returns nullptr on failure (see last_error()). */
        static std::unique_ptr<PlanetTiles> create(const PlanetTilesOptions &options);
        ~PlanetTiles();
        PlanetTiles(const PlanetTiles &) = delete;
        PlanetTiles &operator=(const PlanetTiles &) = delete;

        /**
         * Choose the patches and queue tile loads. `eye` is the camera in the unit sphere's frame (the planet radius is
         * 1). `fov_y` is the vertical field of view in radians and `viewport_height` the height of the view in pixels.
         * `view_projection`, when given, maps that frame to clip space (projection * view * model) and lets patches
         * outside the view be skipped.
         */
        void update(const glm::dvec3 &eye, double fov_y, int viewport_height, const glm::mat4 *view_projection = nullptr);

        /**
         * Draw the patches chosen by update() with the given model matrix (which should scale the unit sphere to the
         * planet radius). `material` supplies the lighting, fog and tint; its texture and normal map are replaced.
         */
        void draw(const glm::mat4 &model, const Material &material = {});

        /** Deepest level the colour map has. */
        int deepest_level() const;
        /** Width in pixels of one tile. */
        int tile_size() const;
        /** True when a specular map was opened. */
        bool has_specular() const;
        /** Changes the brightness of the specular overlay, 0-1, from the next draw (so it can fade with altitude). */
        void set_specular_strength(float strength);
        PlanetTilesStats stats() const;

        /**
         * Numbering helpers, public for tools and tests: how many tile columns and rows a level has, and the
         * UV rectangle (u0, v0, u1, v1) of tile (column, row) of `level` within its ancestor `levels_up` levels shallower.
         */
        static int columns(int level, int base_split = 0) { return 2 << (level + base_split); }
        static int rows(int level, int base_split = 0) { return 1 << (level + base_split); }
        static void ancestor_uv(int column, int row, int levels_up, float rect[4]);

    private:
        PlanetTiles();
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

} // namespace sl
