#pragma once

#include "draw.h"
#include "scene3d.h"

#include <cstdint>
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
    };

    /** Indexed triangle mesh stored on the GPU. Create with create_mesh() or a primitive builder. */
    struct Mesh
    {
        std::uint32_t handle = 0;
        int vertex_count = 0;
        int index_count = 0;
    };

    /** Which triangles draw_mesh() discards. Counter-clockwise triangles are front faces. */
    enum class CullMode
    {
        back,  /**< Discard back faces (default). */
        front, /**< Discard front faces; useful for inside-out hulls and skyboxes. */
        none   /**< Draw both faces. */
    };

    /** Surface appearance for draw_mesh(). The tint and texture multiply the mesh vertex colours. */
    struct Material
    {
        Colour tint{255, 255, 255, 255};
        /** When false the mesh is drawn with its colours unchanged, ignoring the light. */
        bool lit = true;
        /** Optional texture sampled with the mesh UVs and multiplied into the colour. */
        Bitmap *texture = nullptr;
        CullMode cull = CullMode::back;
    };

    /** A single directional light plus ambient term, used by lit materials. */
    struct Light3D
    {
        /** Direction the light travels (from the light toward the scene). */
        glm::vec3 direction{-0.4f, -1.0f, -0.6f};
        /** Minimum brightness of lit surfaces, 0-1. */
        float ambient = 0.25f;
    };

    /**
     * An offscreen 3D drawing target with a depth buffer. Draw into it between
     * begin_surface_3d() and end_surface_3d(), then composite surface_3d_bitmap()
     * into 2D drawing with draw_sprite().
     * Supported on the OpenGL and Vulkan backends; creation fails with an error on others.
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
    /** Set the light used by lit materials on subsequent draws. */
    void set_light_3d(const Light3D &light);

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

} // namespace sl
