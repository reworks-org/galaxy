#pragma once

#include "draw.h"
#include "scene3d.h"
#include "surface3d.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>

namespace sl
{
    /*
     * Chunked level-of-detail terrain for 3D scenes. The terrain is a quadtree of identical small grid meshes
     * ("chunks"); a chunk is split into four children when its screen-space error is too large and merged again
     * when it is small enough. Chunks are generated on worker threads from a TerrainSource and uploaded to the GPU
     * on the thread that calls update(), a few per frame.
     *
     * Two shapes share the same core: a flat square (arenas, flight-sim countryside) and a cube-sphere planet whose
     * close-in terrain can serve a spacecraft sim. All maths is double precision; chunk vertices are stored relative
     * to the chunk centre and draw() takes a floating origin, so planet-sized worlds stay precise.
     *
     * Coordinates are right-handed with +Y up. Flat terrain heights are along +Y; sphere heights are radial.
     */

    using dvec3 = glm::dvec3;

    /** Source of terrain heights. Implementations must be safe to call from several threads at once. */
    class TerrainSource
    {
    public:
        virtual ~TerrainSource() = default;

        /** Height in metres at a flat-terrain world position (x, z). */
        virtual double height(double x, double z) const = 0;
        /**
         * Height in metres above the planet radius in a unit direction from the planet centre. The default maps the
         * direction to longitude and latitude (metres along the equator and meridian) and calls height().
         */
        virtual double height_on_sphere(const dvec3 &direction, double radius) const;
        /**
         * Like height_on_sphere() for a chunk whose grid spacing is cell_size metres, so a source backed by detailed data
         * can read a coarser level for coarse chunks. The default ignores the spacing.
         */
        virtual double height_on_sphere_lod(const dvec3 &direction, double radius, double cell_size) const;
        /** Lowest height this source can return, used to bound chunks before they are built. */
        virtual double min_height() const = 0;
        /** Highest height this source can return. */
        virtual double max_height() const = 0;
        /**
         * Report and clear the rectangle (x0, z0)-(x1, z1) changed since the last call. Returns false when nothing
         * changed. Editable sources override this so the terrain can rebuild the affected chunks.
         */
        virtual bool take_dirty_region(double &x0, double &z0, double &x1, double &z1);
    };

    /** Height from a user function. The function runs on worker threads, so it must be thread-safe. */
    class FunctionSource final : public TerrainSource
    {
    public:
        using Flat = std::function<double(double x, double z)>;
        using Sphere = std::function<double(const dvec3 &direction, double radius)>;

        /** sphere_function is optional; without it the default longitude/latitude mapping is used. */
        FunctionSource(Flat flat_function, double min_height, double max_height, Sphere sphere_function = {});

        double height(double x, double z) const override;
        double height_on_sphere(const dvec3 &direction, double radius) const override;
        double min_height() const override { return min_height_; }
        double max_height() const override { return max_height_; }

    private:
        Flat flat_;
        Sphere sphere_;
        double min_height_;
        double max_height_;
    };

    /** Parameters for NoiseSource: fractal simplex noise. */
    struct NoiseParameters
    {
        int seed = 1337;
        /** Cycles per metre of the first octave (the first octave has a wavelength of 1/frequency metres). */
        double frequency = 1.0 / 2000.0;
        int octaves = 8;
        double lacunarity = 2.0;
        /** Amplitude multiplier per octave. */
        double gain = 0.5;
        /** Height of the first octave's peaks in metres; the total range is about +-amplitude. */
        double amplitude = 800.0;
        /** Added to every height. */
        double base_height = 0.0;
        /** Fold the noise into sharp ridges (mountain ranges) instead of rolling hills. */
        bool ridged = false;
        /** Heights below this are raised to it: a flat sea floor, or 0 to flatten valleys. Ignored when not finite. */
        double floor_height = -1.0e300;
    };

    /** Fractal noise terrain. Sphere terrain samples 3D noise, so there are no seams between cube faces. */
    class NoiseSource final : public TerrainSource
    {
    public:
        explicit NoiseSource(const NoiseParameters &parameters = {});

        double height(double x, double z) const override;
        double height_on_sphere(const dvec3 &direction, double radius) const override;
        double min_height() const override;
        double max_height() const override;
        const NoiseParameters &parameters() const { return parameters_; }

    private:
        double fractal(double x, double y, double z, bool three_d) const;

        NoiseParameters parameters_;
        struct Impl;
        std::shared_ptr<const Impl> impl_;
    };

    /**
     * A regular grid of heights, bilinearly interpolated. Positions outside the grid take the nearest edge height.
     * On a sphere the grid is an equirectangular map (longitude across, latitude down).
     */
    class HeightmapSource : public TerrainSource
    {
    public:
        /**
         * samples are heights in metres, row-major (width * depth values). The grid spans world_width by world_depth
         * metres starting at (origin_x, origin_z).
         */
        HeightmapSource(std::vector<float> samples, int width, int depth, double world_width, double world_depth,
                        double origin_x = 0.0, double origin_z = 0.0);
        /**
         * Build from the red channel of a bitmap, mapped 0..255 to min_height..max_height. With pack_16_bit the green
         * channel is the low byte (value = red * 256 + green), for 16-bit data stored in an RGBA image.
         * Returns nullptr if the bitmap has no CPU pixels.
         */
        static std::shared_ptr<HeightmapSource> from_bitmap(Bitmap *bitmap, double min_height, double max_height,
                                                            double world_width, double world_depth,
                                                            double origin_x = 0.0, double origin_z = 0.0,
                                                            bool pack_16_bit = false);

        double height(double x, double z) const override;
        double height_on_sphere(const dvec3 &direction, double radius) const override;
        double min_height() const override;
        double max_height() const override;
        int grid_width() const { return width_; }
        int grid_depth() const { return depth_; }

    protected:
        double sample_grid(double fx, double fz) const;

        std::vector<float> samples_;
        int width_;
        int depth_;
        double world_width_;
        double world_depth_;
        double origin_x_;
        double origin_z_;
        double min_ = 0.0;
        double max_ = 0.0;
        mutable std::shared_mutex mutex_;
    };

    /** A heightmap that can be edited at runtime. Edited regions make the terrain rebuild the affected chunks. */
    class GridSource final : public HeightmapSource
    {
    public:
        /** A flat grid of `initial` metres. */
        GridSource(int width, int depth, double world_width, double world_depth, double origin_x = 0.0,
                   double origin_z = 0.0, float initial = 0.0f);
        /** Start from an existing grid of heights. */
        GridSource(std::vector<float> samples, int width, int depth, double world_width, double world_depth,
                   double origin_x = 0.0, double origin_z = 0.0);

        /** Set one grid cell (ignored outside the grid). */
        void set_sample(int ix, int iz, float height);
        float sample(int ix, int iz) const;
        /** Add `amount` metres within radius of (x, z), fading to zero at the edge by `falloff` (0 hard, 1 smooth). */
        void raise(double x, double z, double radius, double amount, double falloff = 1.0);
        /** Move heights within radius towards `height` by `strength` (0..1). */
        void flatten(double x, double z, double radius, double height, double strength = 1.0);
        /** Blur heights within radius by `strength` (0..1). */
        void smooth(double x, double z, double radius, double strength = 0.5);

        bool take_dirty_region(double &x0, double &z0, double &x1, double &z1) override;
        /** The bounds expand with edits and never shrink. */
        double min_height() const override;
        double max_height() const override;

    private:
        void mark_dirty(double x0, double z0, double x1, double z1);
        void expand_range(float value);

        std::mutex dirty_mutex_;
        bool dirty_ = false;
        double dirty_x0_ = 0.0, dirty_z0_ = 0.0, dirty_x1_ = 0.0, dirty_z1_ = 0.0;
    };

    /** Context passed to a TerrainConfig::weight_function. */
    struct TerrainSample
    {
        /** Position: world coordinates for flat terrain, the unit direction from the centre for a planet. */
        dvec3 position{0.0};
        double height = 0.0;
        /** Unit surface normal. */
        dvec3 normal{0.0, 1.0, 0.0};
        /** Angle between the normal and straight up, in degrees (0 flat, 90 vertical). */
        double slope_degrees = 0.0;
        /** Grid spacing in metres of the chunk being built; 0 when unknown. */
        double cell_size = 0.0;
    };

    /** An extra texture for a layer, picked per tile by hashed noise to break up repetition. Same tile size as the layer. */
    struct TerrainLayerVariant
    {
        Bitmap *albedo = nullptr;
        Bitmap *normal_map = nullptr;
    };

    /** Most surface types a terrain blends; each has four weights in the vertex colour or eight in total with the extras. */
    constexpr int terrain_max_layers = 8;

    /** One surface layer: a colour and optional textures, placed by height and slope. */
    struct TerrainLayer
    {
        /** Used for the vertex colour (colour-band rendering and distant chunks). */
        Colour colour{128, 128, 128, 255};
        /** Optional textures for splat rendering; not owned. */
        Bitmap *albedo = nullptr;
        Bitmap *normal_map = nullptr;
        /** Metres covered by one texture repeat (the FlightGear xsize). */
        double tile_metres = 16.0;
        /** Further albedo/normal pairs of the same surface; one is chosen per tile in the shader. */
        std::vector<TerrainLayerVariant> variants;

        /** Height band in metres (use +-1e30 for open ends) and how far outside it the layer fades. */
        double min_height = -1.0e30;
        double max_height = 1.0e30;
        double height_blend = 10.0;
        /** Slope band in degrees and fade width. */
        double min_slope = 0.0;
        double max_slope = 90.0;
        double slope_blend = 5.0;
        /** Relative strength where several layers overlap. */
        double strength = 1.0;
        /**
         * How strongly the layer reflects the sun and local lights as a specular highlight, 0 (matte, the default) to 1
         * (water or ice). It only shows when the Material passed to Terrain::draw() has `pbr` set, which also supplies
         * the roughness.
         */
        double specular = 0.0;
    };

    /** Sand, grass, rock and snow spread between low_height and high_height metres. */
    std::vector<TerrainLayer> default_terrain_layers(double low_height = 0.0, double high_height = 600.0);

    /** Whether the terrain is a flat square or a cube-sphere planet. */
    enum class TerrainShape
    {
        flat,
        sphere
    };

    /** How Terrain::set_layer_textures() builds and draws the layer textures. */
    struct TerrainTextureOptions
    {
        /** Beyond fade_start metres each layer blends to its average colour, completely by fade_end (none if fade_end <= fade_start). */
        double fade_start = 150.0;
        double fade_end = 600.0;
        /** Optional large-scale texture that modulates brightness to break up repetition at altitude. */
        Bitmap *macro_texture = nullptr;
        double macro_tile_metres = 2000.0;
        /** 0 disables the macro texture; 1 swings brightness by +-100%. */
        double macro_strength = 0.35;
        /**
         * Use the macro texture as a detail overlay on a coloured terrain instead of splatting layer textures: the vertex
         * colour (TerrainConfig::colour_function, or the layer colours) stays the albedo and the macro texture's
         * brightness modulates it by +-macro_strength, fading out with distance (fade_start, fade_end). No layer needs a
         * texture. The map should be seamless; macro_tile_metres is its repeat size.
         */
        bool colour_detail = false;
        /** Largest layer edge in texels; the array uses a power of two up to this. */
        int max_size = 2048;
    };

    /** Terrain construction parameters. */
    struct TerrainConfig
    {
        TerrainShape shape = TerrainShape::flat;
        /** Flat: edge length in metres of the square, which spans origin.x..origin.x+size by origin.z..origin.z+size. */
        double size = 8192.0;
        /** Flat: world position of the square's minimum corner (y is added to every height). Sphere: planet centre. */
        dvec3 origin{0.0};
        /** Sphere: radius in metres of height zero. */
        double radius = 6.371e6;

        /** Quads along each side of a chunk (a power of two works best); chunks have (cells + 1)^2 vertices. */
        int chunk_cells = 32;
        /** Spacing in metres of the finest chunks; sets the deepest quadtree level. */
        double finest_cell_size = 1.0;
        /** Overrides the deepest level when not negative. */
        int max_level = -1;
        /** A chunk is split while its screen-space error exceeds this many pixels. */
        double max_screen_error = 3.0;
        /** Vertical error of a chunk as a fraction of its cell size. Lower values refine less. */
        double error_scale = 0.5;
        /** Skirt depth as a multiple of the cell size; skirts hide cracks between neighbouring levels. */
        double skirt_scale = 1.0;
        /**
         * Metres after which the vertex UVs wrap, which must be a whole number of every layer's tile. 0 uses the
         * largest layer tile_metres; Terrain::set_layer_textures() sets it itself to fit its macro texture.
         */
        double uv_period_metres = 0.0;

        /** Layers placed by height and slope; empty uses default_terrain_layers() scaled to the source range. */
        std::vector<TerrainLayer> layers;
        /**
         * Optional override of layer weights per vertex (one per layer, at most terrain_max_layers); runs on worker
         * threads, so it must be thread-safe.
         */
        std::function<void(const TerrainSample &sample, float weights[terrain_max_layers])> weight_function;
        /**
         * Put the layer weights in the vertex attributes instead of the blended layer colours. Terrain::set_layer_textures()
         * turns this on by itself while textures are set.
         */
        bool weights_in_vertex_colour = false;
        /**
         * Optional per-vertex surface colour (linear 0-1 rgb), such as from satellite imagery. When it returns true the
         * colour replaces the layer-blended colour of untextured terrain; runs on worker threads, so it must be thread-safe.
         */
        std::function<bool(const TerrainSample &sample, float rgb[3])> colour_function;
        /**
         * Store, per vertex, the offset to the parent chunk's surface so the terrain shader can blend a chunk
         * toward its coarser shape as the camera moves away, hiding level-change pops. Costs one extra grid of
         * height samples per chunk build.
         */
        bool geomorph = true;

        /** Background threads that build chunks: -1 chooses automatically, 0 builds on the calling thread. */
        int worker_threads = -1;
        /** Finished chunks uploaded to the GPU per update() call. */
        int max_uploads_per_frame = 2;
        /** Chunks kept in memory before unused ones are evicted. */
        int max_cached_chunks = 1500;
        /** Frames a chunk must go unused before it can be evicted. */
        int evict_after_frames = 240;
    };

    /** A chunk's place in the quadtree. face is 0 for flat terrain and 0..5 for the cube faces of a planet. */
    struct TerrainNodeKey
    {
        int face = 0;
        int level = 0;
        int x = 0;
        int y = 0;

        bool operator==(const TerrainNodeKey &other) const
        {
            return face == other.face && level == other.level && x == other.x && y == other.y;
        }
    };

    /** CPU-side mesh data for one chunk, produced by build_terrain_chunk(). */
    struct TerrainChunkData
    {
        TerrainNodeKey key;
        /** World position the vertices are relative to. */
        dvec3 center{0.0};
        /** Radius of a sphere around the center that holds every vertex. */
        double bounding_radius = 0.0;
        double min_height = 0.0;
        double max_height = 0.0;
        /**
         * Vertex tangent xyz holds the offset (relative to the chunk center) to the parent chunk's surface, and w is 1.
         * The terrain shader applies it by distance; ordinary materials would read it as a tangent.
         */
        /** Vertices per side of the main grid (the skirt follows after the grid). */
        int grid_vertices = 0;
        std::vector<Vertex3D> vertices;
        std::vector<std::uint32_t> indices;
    };

    /** The camera for LOD selection and culling, in the same double-precision world space as the terrain. */
    struct TerrainView
    {
        dvec3 position{0.0, 100.0, 0.0};
        dvec3 forward{0.0, 0.0, -1.0};
        dvec3 up{0.0, 1.0, 0.0};
        double fov_y_degrees = 60.0;
        double aspect = 16.0 / 9.0;
        /** Height of the render target in pixels; the screen-space error is measured in these pixels. */
        double viewport_height = 720.0;
        /** Chunks further than this are not drawn. */
        double far_distance = 1.0e30;
    };

    /** Build a view from a float camera whose position is replaced by a double-precision world position. */
    TerrainView make_terrain_view(const Camera &camera, const dvec3 &world_position, int viewport_width,
                                  int viewport_height);

    /** Result of Terrain::raycast(). */
    struct TerrainHit
    {
        bool hit = false;
        dvec3 position{0.0};
        dvec3 normal{0.0, 1.0, 0.0};
        double distance = 0.0;
    };

    /** Counters describing the terrain's current state. */
    struct TerrainStats
    {
        /** Chunks drawn by the last draw(). */
        int visible_chunks = 0;
        int visible_triangles = 0;
        /** Chunks with a GPU mesh. */
        int resident_chunks = 0;
        /** Chunks waiting for a worker or for upload. */
        int pending_chunks = 0;
        /** Deepest level among the visible chunks. */
        int deepest_level = 0;
    };

    /** Build one chunk's mesh without a renderer. Thread-safe. Used by Terrain and available for tools and tests. */
    TerrainChunkData build_terrain_chunk(const TerrainConfig &config, const TerrainSource &source,
                                         const TerrainNodeKey &key);

    /** A streaming LOD terrain. Create it with a shape, a source and a config; call update() every frame. */
    class Terrain
    {
    public:
        Terrain(const TerrainConfig &config, std::shared_ptr<TerrainSource> source);
        /** Destroys chunk meshes, so it must run before the renderer is shut down. */
        ~Terrain();
        Terrain(const Terrain &) = delete;
        Terrain &operator=(const Terrain &) = delete;

        const TerrainConfig &config() const;
        const std::shared_ptr<TerrainSource> &source() const;
        /** Deepest quadtree level. */
        int max_level() const;

        /**
         * Turn a planet by `angle` radians about `axis` (default +Y) through config().origin. The chunks stay built in the body-fixed
         * frame; update(), draw() and every query that takes or returns a world direction or position (height_at_direction,
         * normal_at_direction, surface_point, altitude_of, normal_below, raycast) then work in the turned world frame. Zero
         * (the default) leaves the planet fixed. Ignored for flat terrain. Const and atomic so a FlightWorld can drive it
         * from its simulation clock (see FlightBody::spin_phase).
         */
        void set_rotation(double angle, const glm::dvec3 &axis = glm::dvec3(0.0, 1.0, 0.0)) const;
        double rotation() const;
        /** The unit axis set_rotation() turns the planet about. */
        glm::dvec3 rotation_axis() const;
        /** Choose the chunks to draw for the view, request missing chunks and upload finished ones. */
        void update(const TerrainView &view);
        /**
         * Request and keep resident the chunks `view` would draw, without changing what draw() shows. Call it after
         * update() each frame for a place the camera is about to reach, so it is built before arrival.
         */
        void prefetch(const TerrainView &view);
        /**
         * Draw the chunks chosen by the last update() into the active 3D surface. `origin` is subtracted from every
         * position in double precision, so the surface camera must be expressed relative to the same origin
         * (camera.position = view.position - origin). The material's texture, tint and lighting apply to every chunk.
         */
        bool draw(const dvec3 &origin, const Material &material = {}) const;
        /**
         * Texture the terrain from config().layers (at most terrain_max_layers) using GPU texture arrays: every layer
         * wraps, is mipmapped and anisotropically filtered, and repeats every its own tile_metres, so no pre-baked atlas
         * is needed. Layer weights are per vertex, so all surface types are always present. Layers with no albedo are
         * drawn in their flat colour. Each layer's variants are chosen per tile by hashed noise. Layer sizes are
         * unrestricted: they are resampled to one power-of-two size (the largest input, capped by options.max_size).
         * The bitmaps are copied to the GPU and not owned or read again.
         *
         * Needs a renderer with texture arrays (OpenGL and Vulkan); returns false otherwise, or if no layer has an
         * albedo, and vertex colours stay in use. Rebuilds the chunks, so call it at setup after set_gfx_mode().
         */
        bool set_layer_textures(const TerrainTextureOptions &options = {});
        /**
         * As above, but textures the terrain from `layers` instead of config().layers, so the surface textures can change
         * while the terrain lives (for example on entering another region). Only the albedo, normal map, tile_metres and
         * variants are used; the layer count and the weight function (and so the layer order) must stay as configured.
         * The bitmaps are copied to the GPU and not owned or read again. Rebuilds the chunks.
         */
        bool set_layer_textures(const std::vector<TerrainLayer> &layers, const TerrainTextureOptions &options = {});
        /** Go back to vertex colours and free the texture arrays. */
        void clear_layer_textures();
        /** Block until every chunk the last update() asked for has been built and uploaded (for loading screens). */
        void wait_until_ready(const TerrainView &view, int max_updates = 600);

        /** The chunks ideal for the view, ignoring what has been built. Needs no renderer. */
        std::vector<TerrainNodeKey> select_nodes(const TerrainView &view) const;
        TerrainStats stats() const;

        /** Flat terrain: world Y of the surface at (x, z) (origin.y plus the source height). Exact, independent of the chunk LOD. */
        double height_at(double x, double z) const;
        /** Flat terrain: unit normal at (x, z). */
        dvec3 normal_at(double x, double z) const;
        /** Planet: height in metres above the radius in a direction from the centre. */
        double height_at_direction(const dvec3 &direction) const;
        /** Planet: unit normal at a direction from the centre. */
        dvec3 normal_at_direction(const dvec3 &direction) const;
        /** Planet: world position of the surface in a direction from the centre. */
        dvec3 surface_point(const dvec3 &direction) const;
        /** Height above the surface below a world position (flat: along Y; planet: radial). */
        double altitude_of(const dvec3 &position) const;
        /** Surface normal beneath a world position (flat: at x,z; planet: in the direction of the position). */
        dvec3 normal_below(const dvec3 &position) const;
        /** Intersect a ray with the terrain surface (the source heights, not the rendered chunks). */
        TerrainHit raycast(const dvec3 &origin, const dvec3 &direction, double max_distance) const;
        /** Rebuild the chunks overlapping a world rectangle after the source changed (flat terrain). */
        void invalidate_region(double x0, double z0, double x1, double z1);
        /** Rebuild every chunk. */
        void invalidate_all();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

} // namespace sl

namespace std
{
    template <>
    struct hash<sl::TerrainNodeKey>
    {
        size_t operator()(const sl::TerrainNodeKey &key) const noexcept
        {
            std::uint64_t h = static_cast<std::uint64_t>(key.face) * 0x9E3779B97F4A7C15ull;
            h ^= (static_cast<std::uint64_t>(key.level) + 0x7F4A7C15ull + (h << 6) + (h >> 2));
            h ^= (static_cast<std::uint64_t>(static_cast<std::uint32_t>(key.x)) * 0xC2B2AE3D27D4EB4Full) + (h << 6) + (h >> 2);
            h ^= (static_cast<std::uint64_t>(static_cast<std::uint32_t>(key.y)) * 0x165667B19E3779F9ull) + (h << 6) + (h >> 2);
            return static_cast<size_t>(h);
        }
    };
}
