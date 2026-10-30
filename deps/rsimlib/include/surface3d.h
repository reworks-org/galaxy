#pragma once

#include "draw.h"
#include "scene3d.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace sl
{

    /** A mesh vertex: position, normal, RGBA colour (0-1 floats) and texture coordinates (v = 0 is the top of the image). */
    struct Vertex3D
    {
        float position[3] = {0.0f, 0.0f, 0.0f};
        float normal[3] = {0.0f, 1.0f, 0.0f};
        float colour[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float uv[2] = {0.0f, 0.0f};
        /**
         * Tangent-space +U direction (xyz) and bitangent handedness (w, +1 or -1), used by normal maps.
         * Leave all zero to have create_mesh() generate tangents from the positions and UVs; a non-zero w keeps
         * the supplied tangent as is (terrain meshes store their geomorph offset here).
         */
        float tangent[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        /** Weights of terrain layers 4-7 (layers 0-3 are in the colour). Unused by ordinary meshes. */
        float extra[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    };

    /** Indexed triangle mesh stored on the GPU. Create with create_mesh() or a primitive builder. */
    struct Mesh
    {
        std::uint32_t handle = 0;
        int vertex_count = 0;
        int index_count = 0;
        /** CPU copy of the geometry (with generated tangents); filled while set_mesh_cpu_data_retained(true) and always for skinned model meshes. */
        std::vector<Vertex3D> vertices;
        std::vector<std::uint32_t> indices;
    };

    /**
     * Replace the vertices of an existing mesh (the count must match, the indices are unchanged), for CPU-animated
     * geometry such as skinned models. Zero tangents are regenerated. Call at most once per frame per mesh, before drawing it.
     * Does not change Mesh::vertices, which keeps the original geometry.
     */
    bool update_mesh_vertices(Mesh *mesh, const std::vector<Vertex3D> &vertices);

    /** Make create_mesh() keep a CPU copy of each mesh's vertices and indices in Mesh::vertices / Mesh::indices. Off by default. */
    void set_mesh_cpu_data_retained(bool retained);
    bool mesh_cpu_data_retained();

    /** Which triangles draw_mesh() discards. Counter-clockwise triangles are front faces. */
    enum class CullMode
    {
        back,  /**< Discard back faces (default). */
        front, /**< Discard front faces; useful for inside-out hulls and skyboxes. */
        none   /**< Draw both faces. */
    };

    /**
     * GPU texture arrays and per-layer constants for textured terrain, built by Terrain::set_layer_textures().
     * Layer i of the terrain uses `variants[i]` consecutive array layers starting at `first[i]`, repeats every
     * `tile_metres[i]` metres and fades to `average[i]` with distance.
     */
    struct TerrainTextures
    {
        static constexpr int max_layers = 8;
        /** Backend handles of the albedo and (optional, 0 for none) tangent-space normal arrays. */
        std::uint32_t albedo_array = 0;
        std::uint32_t normal_array = 0;
        int first[max_layers] = {};
        int variants[max_layers] = {};
        /** Tile size after snapping so a whole number of tiles fits in `period_metres`. */
        float tile_metres[max_layers] = {};
        int tiles_per_period[max_layers] = {};
        float average[max_layers][3] = {};
        /** Per-layer specular strength (TerrainLayer::specular), used when the material is pbr. */
        float specular[max_layers] = {};
        /** Array layer of the macro-variation texture (negative for none), its tile size and strength. */
        int macro_layer = -1;
        float macro_tile_metres = 1.0f;
        float macro_strength = 0.0f;
    };

    /**
     * Terrain shading, used by Terrain::draw(). Meshes are expected to hold layer weights in the vertex colour
     * (r, g, b, a = layers 0-3) and Vertex3D::extra (layers 4-7), and a vertex position offset in the tangent xyz. The
     * shader moves each vertex toward its coarser-level position between morph_start and morph_end metres from the eye,
     * so level changes do not pop. With `textures` set the UVs are metres (see TerrainTextures) and the layers are
     * sampled per pixel; without it the vertex colour is drawn as is. With Material::pbr set, terrain also gets a
     * specular highlight scaled per layer (TerrainLayer::specular), using Material::roughness; metallic is ignored.
     */
    struct TerrainShading
    {
        bool enabled = false;
        /** Eye position in the mesh's own coordinates. */
        float eye[3] = {0.0f, 0.0f, 0.0f};
        float morph_start = 0.0f;
        float morph_end = 0.0f;
        /** Beyond fade_start the layers blend to their average colour, fully by fade_end (metres). */
        float fade_start = 0.0f;
        float fade_end = 0.0f;
        /**
         * Quaternion (x, y, z, w) the shaders turn mesh coordinates by before the model matrix (a rotating planet).
         * Terrain::draw sets it; `eye` is then in world axes, relative to the chunk.
         */
        float rotation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        /** Not owned; must stay valid during the draw. */
        const TerrainTextures *textures = nullptr;
    };

    /** Global shading style for 3D draws (OpenGL backend only): flat is cheap faceted lighting, wireframe draws edges. */
    enum class RenderStyle3D
    {
        normal,
        flat,
        wireframe
    };
    void set_render_style_3d(RenderStyle3D style);
    RenderStyle3D render_style_3d();

    /** How a material's colour combines with what is already in the framebuffer. */
    enum class MaterialBlend
    {
        /** Replaces the pixel (the default); alpha_cutoff can still cut holes. */
        opaque,
        /** Translucent: the pixel's opacity is tint alpha times vertex alpha times texture alpha. */
        alpha,
        /** Adds the colour, scaled by the same opacity, for glows, fire and light shafts. */
        additive
    };

    /** Surface appearance for draw_mesh(). The tint and texture multiply the mesh vertex colours. */
    struct Material
    {
        Colour tint{255, 255, 255, 255};
        /** When false the mesh is drawn with its colours unchanged, ignoring the light. */
        bool lit = true;
        /** Optional texture sampled with the mesh UVs and multiplied into the colour. */
        Bitmap *texture = nullptr;
        /**
         * Optional tangent-space normal map (RGB = XYZ, +Y up as in glTF) sampled with the mesh UVs.
         * Needs mesh UVs; tangents are generated automatically. Not owned. Only applies to lit materials.
         */
        Bitmap *normal_map = nullptr;
        CullMode cull = CullMode::back;
        /** Apply the environment's fog and haze (see set_environment_3d()). Turn off for sky domes and emissive effects. */
        bool fog = true;
        /**
         * Discard pixels whose texture alpha (times the vertex alpha) is below this, 0-1; 0 draws everything.
         * Gives hard-edged cut-outs for foliage cards and impostors without sorting. Not applied to terrain.
         */
        float alpha_cutoff = 0.0f;
        /**
         * Blending. Blended draws test against depth but do not write it, so draw all opaque geometry first and the
         * blended meshes afterwards, farthest first, as the draws are not sorted for you. Not applied to terrain.
         * With MaterialBlend::additive set `fog` to false unless you want distant glows to fade towards the fog colour.
         */
        MaterialBlend blend = MaterialBlend::opaque;
        /**
         * Physically based shading (glTF metallic-roughness): GGX specular with the scene's directional light, ambient
         * light scaled by occlusion, and emission. Only for lit materials; the texture is the base colour. Available
         * on every backend (the Direct3D 12 path has not been tested on Windows yet).
         */
        bool pbr = false;
        float metallic = 1.0f;
        float roughness = 1.0f;
        /** Optional texture: G multiplies roughness, B multiplies metallic, R is ambient occlusion (glTF layout). Not owned. */
        Bitmap *metallic_roughness = nullptr;
        /** How much of the texture's R channel darkens ambient light, 0-1. */
        float occlusion_strength = 1.0f;
        /** Linear rgb light the surface gives off, added after lighting. */
        float emissive[3] = {0.0f, 0.0f, 0.0f};
        TerrainShading terrain;
    };

    /** A single directional light plus ambient term, used by lit materials. */
    struct Light3D
    {
        /** Direction the light travels (from the light toward the scene). */
        glm::vec3 direction{-0.4f, -1.0f, -0.6f};
        /** Minimum brightness of lit surfaces, 0-1. */
        float ambient = 0.25f;
        /** Colour of the directional light; atmosphere_sun_colour() gives a sunset-aware value. */
        glm::vec3 colour{1.0f, 1.0f, 1.0f};
    };

    /**
     * An offscreen 3D drawing target with a depth buffer. Draw into it between
     * begin_surface_3d() and end_surface_3d(), then composite surface_3d_bitmap()
     * into 2D drawing with draw_sprite().
     * Supported on the OpenGL, Vulkan and Direct3D 12 backends; creation fails with an error on others.
     */
    struct Surface3D
    {
        Bitmap *bitmap = nullptr;
        int samples = 1;
    };

    /**
     * Create a surface, or nullptr (see last_error()) if unsupported or allocation fails.
     * samples requests multisample anti-aliasing (e.g. 4); it is clamped to what the GPU supports.
     */
    Surface3D *create_surface_3d(int width, int height, int samples = 1);
    /** The sample count actually in use (1 when MSAA is off or unsupported). */
    int surface_3d_samples(const Surface3D *surface);
    void destroy_surface_3d(Surface3D *surface);
    /** The surface's colour image, for use with draw_sprite() and similar 2D calls. */
    Bitmap *surface_3d_bitmap(Surface3D *surface);

    /** Direct subsequent 3D drawing to the surface using the camera. Contents are undefined until clear_surface_3d(). */
    bool begin_surface_3d(Surface3D *surface, const Camera &camera);
    void end_surface_3d();
    /** Clear the active surface's colour and depth. */
    void clear_surface_3d(Colour colour);
    /**
     * Clear only the active surface's depth, keeping what is drawn. With set_camera_3d() this lets a scene be drawn in
     * layers with different near and far planes: distant bodies first, then the nearby objects over them. Returns false
     * (and clears nothing) on a backend that cannot.
     */
    bool clear_depth_3d();
    /** Replace the camera used by the draws that follow, within begin_surface_3d() and end_surface_3d(). */
    void set_camera_3d(const Camera &camera);
    /** Set the light used by lit materials on subsequent draws. */
    void set_light_3d(const Light3D &light);
    /** The light most recently passed to set_light_3d(). */
    Light3D get_light_3d();

    /** The most local lights lit materials will use per draw; extra lights passed to set_local_lights_3d() are ignored. */
    constexpr int max_local_lights_3d = 16;
    /** The most local lights that can cast shadows at once. */
    constexpr int max_local_shadow_lights_3d = 4;

    /**
     * A point or spot light placed in the scene, added to the directional light from set_light_3d(). Positions are in
     * the same world space as the camera and model matrices.
     */
    struct LocalLight3D
    {
        glm::vec3 position{0.0f};
        /** Linear rgb colour. */
        glm::vec3 colour{1.0f, 1.0f, 1.0f};
        /** Multiplies colour; 1 lights a surface facing the light at close range to roughly its albedo. */
        float intensity = 1.0f;
        /** Distance in world units at which the light has faded to nothing. */
        float range = 20.0f;
        /** Direction a spotlight points; ignored when outer_angle is 0. */
        glm::vec3 direction{0.0f, -1.0f, 0.0f};
        /** Inner (full strength) and outer (zero) cone half-angles in degrees. A zero outer angle makes a point light. */
        float inner_angle = 0.0f;
        float outer_angle = 0.0f;
        /**
         * Cast shadows. Only the first max_local_shadow_lights_3d lights with this set get a shadow map, rendered by
         * render_local_shadows_3d().
         */
        bool cast_shadow = false;
    };

    /** Replace the point and spot lights used by lit materials on subsequent draws. Pass an empty list to clear. */
    void set_local_lights_3d(const std::vector<LocalLight3D> &lights);
    /** The lights most recently passed to set_local_lights_3d(), truncated to max_local_lights_3d. */
    std::vector<LocalLight3D> get_local_lights_3d();

    /**
     * A sun shadow map: the scene's depth as seen along the Light3D direction, over a square window that follows a focus
     * point. Render it every frame (or whenever the casters or sun move) with begin_shadow_pass_3d(), then enable
     * sampling with set_shadow_map_3d(). Supported on OpenGL and Vulkan; the Direct3D 12 implementation is untested.
     */
    struct ShadowMap3D
    {
        Surface3D *surface = nullptr;
        int size = 0;
        /** World-to-light clip matrix of the last shadow pass. */
        glm::mat4 view_projection{1.0f};
        /** World-space width of one shadow texel, used to offset lookups along the surface normal. */
        float texel_size = 0.0f;
    };

    /** Create a square shadow map (e.g. 2048), or nullptr (see last_error()). */
    ShadowMap3D *create_shadow_map_3d(int size = 2048);
    void destroy_shadow_map_3d(ShadowMap3D *map);
    /**
     * Start drawing shadow casters into the map. The window is a square of side 2 * radius centred on `centre`, looking
     * along get_light_3d().direction. Only opaque meshes drawn with draw_mesh() or draw_mesh_instanced() cast shadows;
     * terrain, blended and unlit draws are skipped. Do not call between begin_surface_3d() and end_surface_3d().
     */
    bool begin_shadow_pass_3d(ShadowMap3D *map, const glm::vec3 &centre, float radius);
    void end_shadow_pass_3d();
    /**
     * Make lit materials drawn from now on receive the map's shadows from the sun (nullptr turns shadows off).
     * `bias` is a depth offset in 0-1 light-depth units that prevents self-shadowing acne.
     */
    void set_shadow_map_3d(const ShadowMap3D *map, float bias = 0.0015f);
    /** The map and bias most recently passed to set_shadow_map_3d(); the map is nullptr when sun shadows are off. */
    const ShadowMap3D *get_shadow_map_3d(float *bias = nullptr);

    /**
     * Shadow maps for point and spot lights: an atlas holding one perspective depth map per spotlight and a six-face
     * cube for each point light (see LocalLight3D::cast_shadow). Supported on OpenGL and Vulkan; the Direct3D 12
     * implementation is untested.
     */
    struct LocalShadowMap3D
    {
        /** Scratch surface each face is rendered into before being copied to the atlas. */
        Surface3D *face = nullptr;
        /** Packed-depth atlas: 3 faces across, 2 faces down per shadow slot. */
        Bitmap *atlas = nullptr;
        int face_size = 0;
        int max_lights = 0;
        /** Number of slots filled by the last render_local_shadows_3d(). */
        int slots_used = 0;
        /** Shadow slot of each light passed to set_local_lights_3d() at the last render, or -1. */
        int light_slot[max_local_lights_3d] = {};
        /** Number of faces rendered for each slot: 1 for a spotlight, 6 for a point light. */
        int slot_faces[max_local_shadow_lights_3d] = {};
        /** Near and far plane of each slot's shadow projection. */
        float slot_near[max_local_shadow_lights_3d] = {};
        float slot_far[max_local_shadow_lights_3d] = {};
        /** World-to-light clip matrix of each slot and cube face. */
        glm::mat4 view_projection[max_local_shadow_lights_3d][6];
    };

    /** Create local light shadow maps with square faces (e.g. 512) for up to max_lights lights, or nullptr (see last_error()). */
    LocalShadowMap3D *create_local_shadow_map_3d(int face_size = 512, int max_lights = max_local_shadow_lights_3d);
    void destroy_local_shadow_map_3d(LocalShadowMap3D *map);
    /**
     * Render the shadow maps for the lights most recently passed to set_local_lights_3d(). `draw_casters` is called once
     * per face (one for each spotlight, six for each point light) and should draw every caster with draw_mesh(); the
     * same rules as begin_shadow_pass_3d() apply. Call it again whenever the lights or casters move, and not between
     * begin_surface_3d() and end_surface_3d().
     */
    bool render_local_shadows_3d(LocalShadowMap3D *map, const std::function<void()> &draw_casters);
    /**
     * Make lit materials drawn from now on receive the shadows of the lights in the map (nullptr turns them off). The
     * lights must be unchanged since render_local_shadows_3d().
     */
    void set_local_shadow_map_3d(const LocalShadowMap3D *map);
    /** The map most recently passed to set_local_shadow_map_3d(). */
    const LocalShadowMap3D *get_local_shadow_map_3d();

    /** Create a mesh from indexed triangles; returns nullptr on failure. */
    Mesh *create_mesh(const std::vector<Vertex3D> &vertices, const std::vector<std::uint32_t> &indices);
    void destroy_mesh(Mesh *mesh);
    /** Primitive builders. All meshes are centred on the origin with outward-facing normals. */
    Mesh *create_cube_mesh(float size = 1.0f, Colour colour = {255, 255, 255, 255});
    Mesh *create_plane_mesh(float width, float depth, Colour colour = {255, 255, 255, 255});
    Mesh *create_sphere_mesh(float radius, int segments = 24, int rings = 16,
                             Colour colour = {255, 255, 255, 255});
    /** Draw a mesh into the active surface. */
    bool draw_mesh(const Mesh *mesh, const glm::mat4 &model, const Material &material = {});

    /** One instance of draw_mesh_instanced(): where to place the mesh and how to colour it. */
    struct InstanceData3D
    {
        /** Model matrix placing this copy in the world. */
        glm::mat4 model{1.0f};
        /** Multiplies the mesh's vertex colours (rgb and alpha), on top of Material::tint. */
        Colour tint{255, 255, 255, 255};
    };

    /** GPU storage for per-instance data, reusable across frames. Create with create_instance_buffer(). */
    struct InstanceBuffer
    {
        std::uint32_t handle = 0;
        /** Most instances the buffer can hold; fixed at creation. */
        std::size_t capacity = 0;
        /** Instances written by the last update_instance_buffer(). */
        std::size_t count = 0;
    };

    /**
     * Allocate room for `capacity` instances; returns nullptr on failure, or when the backend cannot instance.
     * Use this for groups that stay put or change rarely, and pass it to draw_mesh_instanced() every frame.
     */
    InstanceBuffer *create_instance_buffer(std::size_t capacity);
    /**
     * Replace the buffer's contents. Fails if there are more instances than its capacity. Call at most once per frame,
     * before drawing it that frame; the previous contents stay in use by draws already issued on Vulkan and Direct3D 12.
     */
    bool update_instance_buffer(InstanceBuffer *buffer, const std::vector<InstanceData3D> &instances);
    void destroy_instance_buffer(InstanceBuffer *buffer);

    /**
     * Draw every instance in `buffer` with one call using hardware instancing. The view-projection is applied once and
     * each instance's model matrix in the vertex shader, so a buffer stays valid when the camera moves. Material
     * `terrain` shading is not supported. Returns false when the backend cannot instance.
     */
    bool draw_mesh_instanced(const Mesh *mesh, const InstanceBuffer *buffer, const Material &material = {});
    /**
     * Draw `instances` in one call, uploading them for this frame only. Convenient when instances change every frame
     * (it uploads each time); prefer a persistent InstanceBuffer for groups that rarely change.
     */
    bool draw_mesh_instanced(const Mesh *mesh, const std::vector<InstanceData3D> &instances, const Material &material = {});

    /**
     * A list of draws that worker threads can fill while the main thread does other work. Filling a queue touches no
     * rendering state, so each thread should use its own queue; a queue must not be shared between threads. Replay
     * queues on the main thread with submit_render_queue().
     */
    class RenderQueue3D
    {
    public:
        /** Record draw_mesh(). The mesh, material textures and terrain textures must outlive submission. */
        void draw_mesh(const Mesh *mesh, const glm::mat4 &model, const Material &material = {});
        /** Record draw_mesh_instanced() with a persistent buffer. */
        void draw_mesh_instanced(const Mesh *mesh, const InstanceBuffer *buffer, const Material &material = {});
        void clear() { entries_.clear(); }
        std::size_t size() const { return entries_.size(); }

    private:
        friend bool submit_render_queue(const RenderQueue3D &queue);
        struct Entry
        {
            const Mesh *mesh;
            glm::mat4 model;
            Material material;
            const InstanceBuffer *instances;
        };
        std::vector<Entry> entries_;
    };

    /**
     * Issue every draw in `queue`, in the order it was recorded, into the active 3D surface. Call on the main thread
     * between begin_surface_3d() and end_surface_3d(). Returns false if any draw failed (the rest still run). The queue
     * is left intact; call clear() to reuse it. Texture uploads and other shared state are handled here, so this part
     * stays on one thread; what it saves is your own culling and matrix work (done while filling queues) plus the
     * command recording, which Vulkan and Direct3D 12 spread across threads for large batches.
     */
    bool submit_render_queue(const RenderQueue3D &queue);

} // namespace sl
