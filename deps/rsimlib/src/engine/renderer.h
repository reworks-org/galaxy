#pragma once

#include <SDL2/SDL_video.h>

#include "display.h"
#include "draw.h"
#include "surface3d.h"

#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace sl::detail
{
    enum class PrimitiveType
    {
        points,
        lines,
        line_loop,
        triangles,
        triangle_fan
    };

    enum class ShaderLanguage
    {
        glsl,
        spirv,
        dxil
    };

    enum class ShaderStage
    {
        vertex,
        fragment,
        compute
    };

    /** Describes one named field inside a Vulkan custom shader's fragment push-constant block. */
    struct ShaderUniformLayout
    {
        std::string name;
        std::uint32_t offset = 0;
        std::uint32_t size = 0;
    };

    struct ShaderSource
    {
        ShaderLanguage language = ShaderLanguage::glsl;
        std::string text;
        std::vector<std::uint32_t> spirv;
        std::string asset_id;
        std::vector<std::string> vulkan_sampler_names;
        std::vector<ShaderUniformLayout> vulkan_uniforms;

        static ShaderSource glsl(std::string source)
        {
            return {ShaderLanguage::glsl, std::move(source), {}, {}};
        }
        static ShaderSource spirv_binary(std::vector<std::uint32_t> binary)
        {
            return {ShaderLanguage::spirv, {}, std::move(binary), {}};
        }
    };

    enum class TextureFilter
    {
        nearest,
        linear
    };

    struct TextureDesc
    {
        int width = 0;
        int height = 0;
        TextureFilter filter = TextureFilter::nearest;
    };

    /** Per-draw data for Renderer::draw_mesh(). Matrices are column-major, as in GLM. */
    struct MeshDrawParams
    {
        float mvp[16];
        /** Rows of the 3x3 matrix that transforms normals to world space; w holds tint r, g, b. */
        float normal_rows[3][4];
        /** xyz: unit vector pointing toward the light; w: ambient. Unlit draws use (0,0,0,1). */
        float light[4];
        /** Backend texture handle sampled by the mesh, or 0 for plain vertex colours. */
        std::uint32_t texture = 0;
        CullMode cull = CullMode::back;
    };

    struct Vertex2D
    {
        float x = 0.0f, y = 0.0f, u = 0.0f, v = 0.0f;
        float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
    };

    using GLVertex = Vertex2D;

    inline int get_decomposed_vertex_count(PrimitiveType input_primitive, int count)
    {
        if (count <= 0)
            return 0;
        if (input_primitive == PrimitiveType::triangle_fan)
        {
            if (count == 4)
                return 6;
            if (count == 3)
                return 3;
            if (count > 4)
                return (count - 2) * 3;
            return 0;
        }
        if (input_primitive == PrimitiveType::triangles)
            return count;
        if (input_primitive == PrimitiveType::line_loop)
            return count > 1 ? count * 2 : 0;
        if (input_primitive == PrimitiveType::lines)
            return count;
        if (input_primitive == PrimitiveType::points)
            return count;
        return count;
    }

    inline PrimitiveType get_target_batch_primitive(PrimitiveType input_primitive)
    {
        if (input_primitive == PrimitiveType::triangle_fan || input_primitive == PrimitiveType::triangles)
            return PrimitiveType::triangles;
        if (input_primitive == PrimitiveType::line_loop || input_primitive == PrimitiveType::lines)
            return PrimitiveType::lines;
        return PrimitiveType::points;
    }

    inline void append_decomposed_vertices(PrimitiveType input_primitive, const Vertex2D *vertices, int count,
                                           PrimitiveType &out_batch_primitive, std::vector<Vertex2D> &out_batch_vertices)
    {
        if (!vertices || count <= 0)
            return;

        if (input_primitive == PrimitiveType::triangle_fan)
        {
            out_batch_primitive = PrimitiveType::triangles;
            if (count == 4)
            {
                const Vertex2D quad[6] = {
                    vertices[0], vertices[1], vertices[2],
                    vertices[0], vertices[2], vertices[3]};
                out_batch_vertices.insert(out_batch_vertices.end(), quad, quad + 6);
            }
            else if (count == 3)
            {
                out_batch_vertices.insert(out_batch_vertices.end(), vertices, vertices + 3);
            }
            else if (count > 4)
            {
                for (int i = 1; i < count - 1; ++i)
                {
                    out_batch_vertices.push_back(vertices[0]);
                    out_batch_vertices.push_back(vertices[i]);
                    out_batch_vertices.push_back(vertices[i + 1]);
                }
            }
        }
        else if (input_primitive == PrimitiveType::triangles)
        {
            out_batch_primitive = PrimitiveType::triangles;
            out_batch_vertices.insert(out_batch_vertices.end(), vertices, vertices + count);
        }
        else if (input_primitive == PrimitiveType::line_loop)
        {
            out_batch_primitive = PrimitiveType::lines;
            if (count > 1)
            {
                for (int i = 0; i < count; ++i)
                {
                    out_batch_vertices.push_back(vertices[i]);
                    out_batch_vertices.push_back(vertices[(i + 1) % count]);
                }
            }
        }
        else if (input_primitive == PrimitiveType::lines)
        {
            out_batch_primitive = PrimitiveType::lines;
            out_batch_vertices.insert(out_batch_vertices.end(), vertices, vertices + count);
        }
        else if (input_primitive == PrimitiveType::points)
        {
            out_batch_primitive = PrimitiveType::points;
            out_batch_vertices.insert(out_batch_vertices.end(), vertices, vertices + count);
        }
    }

    /** Internal backend boundary for window/context and presentation ownership. */
    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        /** Configure SDL window attributes before the window is created. */
        virtual void configure_window() = 0;
        /** Return the SDL window flags required by this backend. */
        virtual std::uint32_t window_flags() const = 0;
        /** Create the backend context for an existing SDL window. */
        virtual bool initialise(SDL_Window *window, std::string &error) = 0;
        /** Release backend resources. */
        virtual void shutdown() = 0;
        /** Resize the backend drawable viewport. */
        virtual bool resize(int width, int height, std::string &error) = 0;
        /** Set the backend presentation interval. */
        virtual bool set_vsync(bool enabled) = 0;
        /** Whether present() currently blocks to pace frames to the display refresh.
         * Used to avoid double frame-pacing against the software frame limiter. */
        virtual bool vsync_active() const { return false; }
        /** Present the current backend framebuffer. */
        virtual void present() = 0;
        /** Wait for all submitted backend graphics work to complete. */
        virtual void wait_idle() {}
        /** Begin/end a backend frame around command recording. */
        virtual bool begin_frame(std::string &error) = 0;
        virtual bool end_frame(std::string &error) = 0;
        /** Return the native context for integrations such as ImGui. */
        virtual SDL_GLContext native_context() const = 0;
        /** Create an RGBA texture with backend-managed sampling state. */
        virtual bool create_texture(const TextureDesc &description, std::uint32_t &texture) = 0;
        /** Upload RGBA8 pixels into a texture. */
        virtual bool upload_texture(std::uint32_t texture, int width, int height,
                                    const std::uint8_t *pixels) = 0;
        /** Download RGBA8 pixels from a texture into host memory. */
        virtual bool download_texture(std::uint32_t texture, int width, int height,
                                      std::uint8_t *out_pixels) = 0;
        /** Destroy a texture handle. */
        virtual void destroy_texture(std::uint32_t texture) = 0;
        /** Create a color render target and return its texture and framebuffer handles. */
        virtual bool create_render_target(int width, int height, std::uint32_t &texture,
                                          std::uint32_t &framebuffer) = 0;
        /** Destroy a render target's framebuffer and texture handles. */
        virtual void destroy_render_target(std::uint32_t texture, std::uint32_t framebuffer) = 0;
        virtual bool begin_render_target(std::uint32_t framebuffer, int width, int height,
                                         std::string &error) = 0;
        virtual bool end_render_target(std::string &error) = 0;
        /** Read back RGBA8 pixels from a render target (or framebuffer 0 for default display) into host memory. */
        virtual bool download_render_target(std::uint32_t framebuffer, int width, int height,
                                            std::uint8_t *out_pixels) = 0;
        /** Create a colour render target that also has a depth buffer; false if unsupported. */
        // samples is the requested MSAA count on entry and the count actually used on return.
        virtual bool create_render_target_3d(int, int, int &, std::uint32_t &, std::uint32_t &) { return false; }
        /** Upload an indexed triangle mesh; false if 3D meshes are unsupported. */
        virtual bool create_mesh(const Vertex3D *, std::size_t, const std::uint32_t *, std::size_t,
                                 std::uint32_t &) { return false; }
        virtual void destroy_mesh(std::uint32_t) {}
        /** Draw a mesh into the active render target, depth tested. */
        virtual bool draw_mesh(std::uint32_t, const MeshDrawParams &) { return false; }
        /** Clear the active render target's colour and depth. */
        virtual bool clear_3d(float, float, float, float) { return false; }
        /** Compile and link a vertex/fragment shader program. */
        virtual bool create_shader(const ShaderSource &vertex_source, const ShaderSource &fragment_source,
                                   std::uint32_t &program, std::string &error) = 0;
        /** Compile and link a compute shader program. */
        virtual bool create_compute_shader(const ShaderSource &source, std::uint32_t &program,
                                           std::string &error) = 0;
        /** Create a backend shader module from a SPIR-V artifact. */
        virtual bool create_shader_module(ShaderStage stage, const ShaderSource &source,
                                          std::uint32_t &module, std::string &error) = 0;
        /** Destroy a shader program. */
        virtual void destroy_shader(std::uint32_t program) = 0;
        /** Bind or unbind a shader program. */
        virtual bool use_shader(std::uint32_t program) = 0;
        virtual void stop_shader() = 0;
        /** Dispatch a compute shader. */
        virtual bool dispatch_compute(std::uint32_t program, unsigned int groups_x,
                                      unsigned int groups_y, unsigned int groups_z) = 0;
        virtual bool set_shader_int(std::uint32_t program, const char *name, int value) = 0;
        virtual bool set_shader_float(std::uint32_t program, const char *name, float value) = 0;
        virtual bool set_shader_float2(std::uint32_t program, const char *name, float x, float y) = 0;
        virtual bool set_shader_int2(std::uint32_t program, const char *name, int x, int y) = 0;
        virtual bool set_shader_float3(std::uint32_t program, const char *name, float x, float y, float z) = 0;
        virtual bool set_shader_mat4(std::uint32_t program, const char *name, const float *matrix) = 0;
        /** Initialize and release the backend's default 2D submission resources. */
        virtual bool initialise_2d() = 0;
        virtual void shutdown_2d() = 0;
        /** Bind the default 2D pipeline and projection. */
        virtual bool begin_2d(int width, int height) = 0;
        virtual bool begin_shader_2d(std::uint32_t program, int width, int height) = 0;
        virtual void set_premultiplied_alpha(bool enabled) = 0;
        /** Select the blend mode used by subsequent submit_2d() calls (points primitive only on Vulkan). */
        virtual void set_blend_mode(BlendMode mode) = 0;
        virtual bool clear_frame(float red, float green, float blue, float alpha) = 0;
        /** Submit colored textured vertices using the backend's 2D pipeline. */
        virtual void submit_2d(PrimitiveType primitive_mode, const Vertex2D *vertices,
                               int count, std::uint32_t texture) = 0;
        /** Flush any batched 2D vertices to the GPU. */
        virtual void flush_2d() {}
        /** Backend-neutral storage-buffer operations used by compute effects. */
        virtual bool create_storage_buffer(std::size_t size, std::uint32_t &buffer) = 0;
        virtual void destroy_storage_buffer(std::uint32_t buffer) = 0;
        virtual bool upload_storage_buffer(std::uint32_t buffer, std::size_t size,
                                           const void *data, bool preserve_storage) = 0;
        virtual bool readback_storage_buffer(std::uint32_t buffer, std::size_t offset,
                                             std::size_t size, void *out_data) = 0;
        virtual void bind_storage_buffer(unsigned int binding, std::uint32_t buffer) = 0;
        virtual void bind_texture_unit(unsigned int unit, std::uint32_t texture) = 0;
        virtual void bind_storage_texture(unsigned int binding, std::uint32_t texture) = 0;
        virtual void storage_barrier() = 0;
    };

    /** Create the currently selected renderer backend. */
    std::unique_ptr<Renderer> create_renderer();
    /** Return the active renderer, or nullptr before display initialization. */
    Renderer *active_renderer();
}
