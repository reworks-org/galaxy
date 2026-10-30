#pragma once

#include "draw.h"
#include "surface3d.h"
#include "terrain.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace sl
{
    /** CPU-side triangle geometry, the input to baking. Mesh only keeps GPU buffers, so keep the data you bake from. */
    struct MeshData
    {
        std::vector<Vertex3D> vertices;
        std::vector<std::uint32_t> indices;
    };

    /** Append `part` to `into`, offsetting its indices. */
    void append_mesh_data(MeshData &into, const MeshData &part);

    /**
     * One placed copy of an object. Objects stand on their local up axis, so on a planet pass the surface normal.
     */
    struct Instance3D
    {
        /** Base of the object. */
        glm::vec3 position{0.0f};
        /** Rotation about `up`, in radians. */
        float yaw = 0.0f;
        float scale = 1.0f;
        glm::vec3 up{0.0f, 1.0f, 0.0f};
        /** Multiplies the vertex colours, e.g. for per-tree colour variation. */
        Colour tint{255, 255, 255, 255};
    };

    /**
     * Merge a copy of `source` for every instance into one mesh, so a whole group draws with a single draw_mesh().
     * Instance positions are used as given; draw the result with a translation if they are relative to a local origin.
     * This bakes the transforms on the CPU: build once per group, not per frame. Use it for static groups that are
     * culled or lit as one piece; use draw_mesh_instanced() when instances move or change, or to avoid duplicating the
     * vertex data. Returns nullptr if there is nothing to build.
     */
    Mesh *bake_instances(const MeshData &source, const std::vector<Instance3D> &instances);

    /**
     * A far-LOD stand-in for an object: pre-rendered views from evenly spaced compass directions, drawn as flat
     * cut-out quads. Objects stand on their local up axis and are viewed from the side.
     */
    struct Impostor
    {
        /** One surface per view, each holding the object over a transparent background. */
        std::vector<Surface3D *> views;
        /** World size of the square quad at scale 1, and the height of its centre above the object's base. */
        float size = 1.0f;
        float centre_height = 0.5f;
        /** Alpha cutoff to use when drawing the views. */
        float alpha_cutoff = 0.4f;
    };

    /**
     * Render `source` from `views` directions (8 is a good default) into small surfaces. Call it during setup, not
     * between begin_surface_3d() and end_surface_3d(). `material` supplies texture and tint; lighting is baked in with
     * a fixed light, so the impostor is drawn unlit. Returns nullptr on failure (see last_error()).
     */
    Impostor *bake_impostor(const MeshData &source, const Material &material = {}, int views = 8, int tile_size = 128);
    void destroy_impostor(Impostor *impostor);
    /** The view that best matches a viewer in direction `to_viewer` (unit vector from the object), for objects with the given up. */
    int impostor_view(const Impostor &impostor, const glm::vec3 &to_viewer, const glm::vec3 &up = {0.0f, 1.0f, 0.0f});
    /**
     * Flat quads facing the direction of `view` for every instance, for drawing with impostor_material(). Group
     * instances by impostor_view() of their direction from the camera. Instance yaw is ignored.
     */
    Mesh *bake_billboards(const Impostor &impostor, const std::vector<Instance3D> &instances, int view);
    /** The material that draws bake_billboards() quads of the given view. */
    Material impostor_material(const Impostor &impostor, int view);

    /** A placed object that something walking on the terrain can bump into: a vertical cylinder standing on `position`. */
    struct ClutterObstacle
    {
        dvec3 position{0.0};
        double radius = 0.0;
        double height = 0.0;
        int species = 0;
    };

    /** One kind of object scattered over terrain by a ClutterLayer, with the rules that decide where it grows. */
    struct ClutterSpecies
    {
        /** Geometry of one object standing on y = 0 (for example from make_conifer_data() in props3d.h). */
        MeshData mesh;
        Material material;
        /** Draw a cheap billboard instead of the mesh beyond near_distance. Without it nothing is drawn there. */
        bool impostor = true;
        int impostor_views = 8;
        int impostor_tile = 128;

        /** Objects per square metre where every rule is satisfied. */
        float density = 0.002f;
        /** Height above the terrain's base level (the source height; sea level on a planet), in metres. */
        float min_height = -1.0e30f;
        float max_height = 1.0e30f;
        /** Ground slope range in degrees (0 flat, 90 vertical). */
        float min_slope = 0.0f;
        float max_slope = 35.0f;
        float min_scale = 0.8f;
        float max_scale = 1.3f;
        /** 0 stands objects straight up, 1 tilts them to the surface normal. */
        float lean = 0.0f;
        /** Random brightness variation of each object, 0-1. */
        float tint_variation = 0.15f;
        /** Full meshes are drawn closer than this, impostors up to far_distance, then nothing. */
        float near_distance = 150.0f;
        float far_distance = 900.0f;
        /** Optional extra rule: the chance (0-1) that a candidate spot is used, e.g. to keep trees off snow. */
        std::function<float(const TerrainSample &sample)> suitability;
        /** Whether obstacles() reports this species. */
        bool solid = true;
        /** Changes the arrangement; species with different seeds do not line up. */
        std::uint32_t seed = 1;
    };

    struct ClutterConfig
    {
        /** Side of a square cell in metres. Each cell is generated and drawn as one batch per species. */
        float cell_size = 64.0f;
        /** Cells kept built (at least those in range); the ones unused for longest are dropped first. */
        int max_cached_cells = 1500;
        /** Time per update() spent generating cells and baking meshes, in milliseconds; spreads the cost over frames. */
        double time_budget_ms = 4.0;
    };

    struct ClutterStats
    {
        int cached_cells = 0;
        int drawn_meshes = 0;
        int drawn_objects = 0;
        int drawn_billboards = 0;
    };

    /**
     * Scatters objects over a Terrain: trees, rocks and the like, placed deterministically by height, slope and density
     * so they stay where they are. Works on flat terrain and on planets (objects stand on the local up).
     *
     * The ground is divided into square cells. Cells near the camera get a mesh holding every object in them; farther
     * cells get billboards of an impostor facing the camera, so a forest costs a few draw calls. All work is done in
     * update() on the calling thread, a few cells per frame; draw() only draws. Everything needs a graphics context.
     * The terrain must outlive the layer.
     */
    class ClutterLayer
    {
    public:
        explicit ClutterLayer(const Terrain &terrain, const ClutterConfig &config = {});
        ~ClutterLayer();
        ClutterLayer(const ClutterLayer &) = delete;
        ClutterLayer &operator=(const ClutterLayer &) = delete;

        /**
         * Add a species and bake its impostor. Call outside begin_surface_3d()/end_surface_3d(). Returns the species
         * index, or -1 on failure (see last_error()).
         */
        int add_species(const ClutterSpecies &species);
        int species_count() const;

        /** Build what the view needs and choose what to draw. Call each frame, outside begin_surface_3d(). */
        void update(const TerrainView &view);
        /**
         * Billboards are baked with a fixed light and drawn unlit, so they stay as bright as by day. This scales them
         * (0 black, 1 as baked) to follow the scene's light, for example at night. Meshes are lit normally and unaffected.
         */
        void set_impostor_light(float brightness);
        /** Draw the chosen objects. `origin` is the same camera origin passed to Terrain::draw(). */
        bool draw(const dvec3 &origin) const;

        /** Forget everything built, e.g. after the terrain was reshaped. Rebuilt over the next frames. */
        void invalidate_all();
        /** Flat terrain: forget the cells overlapping a world rectangle. */
        void invalidate_region(double x0, double z0, double x1, double z1);
        ClutterStats stats() const;
        /**
         * Objects within `range` of a world point, from the cells already built (those near the last update() view). Each is
         * a cylinder: the footprint of the species' mesh near the ground and its full height, times the instance's scale.
         */
        void obstacles(const dvec3 &point, double range, std::vector<ClutterObstacle> &out) const;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

} // namespace sl
