#pragma once

#include <SDL3/SDL_video.h>

#include "display.h"
#include "draw.h"
#include "surface3d.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <limits>
#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace sl::detail
{
    class VulkanContext;

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
        bool render_target = false;
        float anisotropy = 1.0f;
        /** Allocate a full mip chain; upload_texture() of the whole texture then fills every level. */
        bool mipmaps = false;
    };

    /** Block-compressed texture formats (BC1 = DXT1, BC3 = DXT5). All store colour as unsigned normalised data. */
    enum class CompressedFormat
    {
        bc1,
        bc3,
        bc7
    };

    /** Description of a block-compressed 2D texture whose mip levels are supplied by the caller. */
    struct CompressedTextureDesc
    {
        int width = 0;
        int height = 0;
        CompressedFormat format = CompressedFormat::bc1;
        /** Number of levels supplied in the data, largest first and tightly packed. */
        int mip_levels = 1;
        TextureFilter filter = TextureFilter::linear;
        float anisotropy = 1.0f;
    };

    /** Bytes in one level of a block-compressed image: 4x4 blocks of 8 (BC1) or 16 bytes. */
    inline std::size_t compressed_level_byte_count(CompressedFormat format, int width, int height)
    {
        const std::size_t blocks = static_cast<std::size_t>((std::max(width, 1) + 3) / 4) *
                                   static_cast<std::size_t>((std::max(height, 1) + 3) / 4);
        return blocks * (format == CompressedFormat::bc1 ? 8u : 16u);
    }

    /** Total bytes of every supplied level of a compressed texture, or 0 if the description is invalid. */
    inline std::size_t compressed_texture_byte_count(const CompressedTextureDesc &description)
    {
        if (description.width <= 0 || description.height <= 0 || description.mip_levels <= 0)
            return 0;
        std::size_t total = 0;
        for (int level = 0; level < description.mip_levels; ++level)
            total += compressed_level_byte_count(description.format, std::max(description.width >> level, 1),
                                                 std::max(description.height >> level, 1));
        return total;
    }

    /** Number of levels in a full mip chain for a width x height texture. */
    inline int mip_level_count(int width, int height)
    {
        int levels = 1;
        for (int size = std::max(width, height); size > 1; size >>= 1)
            ++levels;
        return levels;
    }

    /**
     * Box-filter the mip levels 1..N-1 of a tightly packed RGBA8 image. The levels are returned one after another
     * with no padding; level i is max(width >> i, 1) by max(height >> i, 1). Colour is alpha-weighted so
     * transparent pixels do not darken edges.
     */
    std::vector<std::uint8_t> build_mip_chain(const std::uint8_t *pixels, int width, int height);

    /** A mipmapped, repeating, anisotropically filtered RGBA8 texture array. */
    struct TextureArrayDesc
    {
        int width = 0;
        int height = 0;
        int layers = 0;
        int mip_levels = 1;
    };

    /** Byte offset of a mip level's first layer in the pixel data of create_texture_array(). */
    inline std::size_t texture_array_level_offset(const TextureArrayDesc &desc, int level)
    {
        std::size_t offset = 0;
        for (int i = 0; i < level; ++i)
            offset += static_cast<std::size_t>(std::max(desc.width >> i, 1)) * std::max(desc.height >> i, 1) * 4 *
                      static_cast<std::size_t>(desc.layers);
        return offset;
    }

    inline std::size_t texture_array_byte_count(const TextureArrayDesc &desc)
    {
        return texture_array_level_offset(desc, desc.mip_levels);
    }

    /** Number of surface types a terrain draw can blend (weights are carried in two vertex attributes). */
    constexpr int terrain_layer_count = 8;
    /** Shadow-casting local lights, the vec4 index where their matrices start, and the size of MeshDrawParams::local_lights. */
    constexpr int local_shadow_slots = 4;
    constexpr int local_shadow_matrix_base = 64;
    constexpr int local_light_vec4s = local_shadow_matrix_base + local_shadow_slots * 6 * 4;

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
        /** Backend handle of the tangent-space normal map; a flat-normal texture when the material has none. */
        std::uint32_t normal_texture = 0;
        CullMode cull = CullMode::back;
        /**
         * Blending: 0 opaque, 1 alpha (src alpha, one minus src alpha), 2 additive. Blended draws do not write depth. The
         * shaders output alpha (vertex alpha times texture alpha times emissive[3]) only when emissive[3] is above 0.
         */
        std::uint32_t blend = 0;
        /** rgb: colour fogged surfaces fade to. */
        float fog_colour[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        /** rgb: extinction per metre for each channel (all zero means no fog). */
        float fog_beta[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        /** x, y: tan(fov/2) scaled by aspect and 1; z, w: 1 / viewport width and height. Turns depth into distance. */
        float fog_view[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        /** rgb: colour of the directional light. */
        float light_colour[4] = {1.0f, 1.0f, 1.0f, 0.0f};
        /**
         * Physically based shading, used when pbr[0] is 1: metallic, roughness and occlusion strength in y, z, w; rgb of
         * `emissive`. material_texture (0 for white) holds occlusion, roughness and metalness in R, G and B. The camera
         * vectors are in world space: right and up are scaled by tan(fov/2) (and the aspect ratio for right), so the view
         * ray through a pixel is forward + ndc.x * right + ndc.y * up. Backends without PBR ignore all of these.
         */
        std::uint32_t style = 0;
        float pbr[4] = {0.0f, 1.0f, 1.0f, 1.0f};
        float emissive[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        float camera_right[4] = {};
        float camera_up[4] = {};
        float camera_forward[4] = {};
        /**
         * World-space eye position in xyz and the number of local lights in w. Each local light is four vec4s:
         * [0] position xyz and range, [1] colour (already multiplied by intensity) and cos(outer cone) or -1 for a point
         * light, [2] spot direction xyz and 1 / (cos(inner) - cos(outer)), [3] shadow slot in x (-1 for none) and the
         * near and far planes of its shadow projection in y and z. Needs the camera vectors above.
         * The array continues at vec4 local_shadow_matrix_base with the shadow matrices: slot s, cube face f is the
         * mat4 (four column vec4s) starting at local_shadow_matrix_base + (s * 6 + f) * 4.
         */
        float camera_position[4] = {};
        float local_lights[local_light_vec4s][4] = {};
        /** World-to-light clip matrix for sun shadows. */
        float shadow_matrix[16] = {};
        /** x: 0 none, 1 receive shadows, 2 shadow-pass draw that writes packed depth; y: depth bias; z: texel world size. */
        float shadow[4] = {};
        /** Backend texture handle of the shadow map colour image (packed depth), or 0. shadow[3] is the number of local shadow slots in use. */
        std::uint32_t shadow_texture = 0;
        /** Atlas of local light shadow faces (3 faces wide, 2 per slot tall), packed depth like shadow_texture, or 0. */
        std::uint32_t local_shadow_texture = 0;
        std::uint32_t material_texture = 0;
        /**
         * Terrain splatting. When terrain_albedo is not 0 the mesh is drawn with the terrain pipeline instead: texture and
         * normal_texture are ignored, the vertex colour and Vertex3D::extra hold the eight layer weights, and the
         * vertex UVs are metres. terrain_normals may be 0 (no normal maps). normal_rows carries the terrain parameters as for
         * untextured terrain: [0].w = -1, [1].xyz eye, [1].w morph start, [2].x morph end, [2].yz fade start and end.
         */
        std::uint32_t terrain_albedo = 0;
        std::uint32_t terrain_normals = 0;
        /** Per surface type: x first array layer, y variant count, z tile metres, w tiles per UV period. */
        float terrain_layers[terrain_layer_count][4] = {};
        /** Per surface type: rgb average colour, which distant ground fades to. */
        float terrain_average[terrain_layer_count][4] = {};
        /** x: macro texture array layer (negative for none), y: macro tile metres, z: macro strength. */
        float terrain_macro[4] = {-1.0f, 1.0f, 0.0f, 0.0f};
    };

    /** One instance of an instanced mesh draw: a column-major model matrix and an rgba tint multiplying the vertex colours. */
    struct InstanceGpu
    {
        float model[16];
        float tint[4];
    };

    /** Per-draw data for Renderer::draw_sky(): vec4 rows laid out as documented in atmosphere.cpp. */
    struct SkyDrawParams
    {
        static constexpr int rows = 28;
        float data[rows][4] = {};
    };

    struct Vertex2D
    {
        float x = 0.0f, y = 0.0f, u = 0.0f, v = 0.0f;
        float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
    };

    using GLVertex = Vertex2D;

    /** A 2D clip rectangle in drawing coordinates (the space passed to begin_2d). */
    struct ClipRect2D
    {
        bool enabled = false;
        float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;
    };

    /**
     * Convert a clip rectangle to a top-left-origin pixel rectangle inside a framebuffer of
     * framebuffer_width x framebuffer_height pixels, given the drawing-space size and screen offset.
     * The result is clamped to the framebuffer and may be empty.
     */
    inline void clip_rect_to_pixels(const ClipRect2D &clip, int drawing_width, int drawing_height,
                                    int framebuffer_width, int framebuffer_height,
                                    float offset_x, float offset_y,
                                    int &out_x, int &out_y, int &out_width, int &out_height)
    {
        const float scale_x = drawing_width > 0 ? static_cast<float>(framebuffer_width) / drawing_width : 1.0f;
        const float scale_y = drawing_height > 0 ? static_cast<float>(framebuffer_height) / drawing_height : 1.0f;
        int left = static_cast<int>((clip.x + offset_x) * scale_x);
        int top = static_cast<int>((clip.y + offset_y) * scale_y);
        int right = static_cast<int>((clip.x + clip.width + offset_x) * scale_x + 0.999f);
        int bottom = static_cast<int>((clip.y + clip.height + offset_y) * scale_y + 0.999f);
        left = left < 0 ? 0 : (left > framebuffer_width ? framebuffer_width : left);
        top = top < 0 ? 0 : (top > framebuffer_height ? framebuffer_height : top);
        right = right < left ? left : (right > framebuffer_width ? framebuffer_width : right);
        bottom = bottom < top ? top : (bottom > framebuffer_height ? framebuffer_height : bottom);
        out_x = left;
        out_y = top;
        out_width = right - left;
        out_height = bottom - top;
    }

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
            {
                if (count - 2 > std::numeric_limits<int>::max() / 3)
                    return 0;
                return (count - 2) * 3;
            }
            return 0;
        }
        if (input_primitive == PrimitiveType::triangles)
            return count;
        if (input_primitive == PrimitiveType::line_loop)
        {
            if (count <= 1 || count > std::numeric_limits<int>::max() / 2)
                return 0;
            return count * 2;
        }
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
        /** Create the backend context for an existing SDL window. */
        virtual bool initialise(SDL_Window *window, std::string &error) = 0;
        /** Release backend resources. */
        virtual void shutdown() = 0;
        /** Resize the backend drawable viewport. */
        virtual bool resize(int width, int height, std::string &error) = 0;
        /** Present the current backend framebuffer. */
        virtual void present() = 0;
        /** Wait for all submitted backend graphics work to complete. */
        virtual void wait_idle() {}
        /** Begin/end a backend frame around command recording. */
        virtual bool begin_frame(std::string &error) = 0;
        virtual bool end_frame(std::string &error) = 0;
        /** Return the native context for integrations such as ImGui. */
        virtual SDL_GLContext native_context() const = 0;
        /** Return the Vulkan context for internal Vulkan integrations, if active. */
        virtual VulkanContext *vulkan_context() { return nullptr; }
        /** Dear ImGui rendering hooks for backends that own their ImGui renderer (D3D12). */
        virtual bool gui_backend_init() { return false; }
        virtual void gui_backend_new_frame() {}
        virtual void gui_backend_render() {}
        virtual void gui_backend_shutdown() {}
        /** Create an RGBA texture with backend-managed sampling state. */
        virtual bool create_texture(const TextureDesc &description, std::uint32_t &texture) = 0;
        /** Upload RGBA8 pixels into a texture. */
        virtual bool upload_texture(std::uint32_t texture, int width, int height,
                                    const std::uint8_t *pixels) = 0;
        /** Whether create_texture_compressed() accepts this format on the active device. */
        virtual bool supports_compressed_format(CompressedFormat) const { return false; }
        /** Create a read-only texture straight from block-compressed data holding every mip level. */
        virtual bool create_texture_compressed(const CompressedTextureDesc &, const std::uint8_t *, std::uint32_t &texture)
        {
            texture = 0;
            return false;
        }
        /** Download RGBA8 pixels from a texture into host memory. */
        virtual bool download_texture(std::uint32_t texture, int width, int height,
                                      std::uint8_t *out_pixels) = 0;
        /** Destroy a texture handle. */
        virtual void destroy_texture(std::uint32_t texture) = 0;
        /** Most layers a texture array may have, or 0 if texture arrays (and so textured terrain) are unsupported. */
        virtual int max_texture_array_layers() const { return 0; }
        /** Largest width or height a 2D texture may have. */
        virtual int max_texture_size() const { return 16384; }
        /**
         * Create a texture array with repeat wrapping, trilinear filtering and the best anisotropic filtering available.
         * `pixels` holds RGBA8 for every mip level in turn, and within a level every layer in turn (see
         * texture_array_byte_count()). The array is bound for sampling only by draw_mesh().
         */
        virtual bool create_texture_array(const TextureArrayDesc &, const std::uint8_t *, std::uint32_t &) { return false; }
        virtual void destroy_texture_array(std::uint32_t) {}
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
        /** Replace all vertices of an existing mesh (same count as at creation), e.g. after CPU skinning. At most once per frame. */
        virtual bool update_mesh_vertices(std::uint32_t, const Vertex3D *, std::size_t) { return false; }
        /** Draw a mesh into the active render target, depth tested. */
        virtual bool draw_mesh(std::uint32_t, const MeshDrawParams &) { return false; }
        /** Draw the procedural sky behind everything already in the active 3D target; false if unsupported. */
        /**
         * Per-instance data for draw_mesh_instanced(). Buffers have a fixed capacity and may be updated at most once per
         * frame, before they are drawn that frame. Backends without instancing leave these unimplemented.
         */
        virtual bool create_instance_buffer(std::size_t, std::uint32_t &) { return false; }
        virtual bool update_instance_buffer(std::uint32_t, const InstanceGpu *, std::size_t) { return false; }
        virtual void destroy_instance_buffer(std::uint32_t) {}
        /**
         * Draw `count` instances of a mesh. params.mvp is the view-projection matrix (each instance's model matrix is
         * applied in the shader), params.normal_rows[i][3] still holds the material tint, and terrain is not supported.
         */
        virtual bool draw_mesh_instanced(std::uint32_t, const MeshDrawParams &, std::uint32_t, std::size_t) { return false; }
        virtual bool draw_sky(const SkyDrawParams &) { return false; }
        /** Clear the active render target's colour and depth. */
        virtual bool clear_3d(float, float, float, float) { return false; }
        /** Clear only the active render target's depth. */
        virtual bool clear_depth_3d() { return false; }
        /**
         * Build the pipeline states 3D drawing needs for a render-target sample count ahead of the first draw, so the
         * first frame does not stall compiling them. Backends without pipeline objects do nothing.
         */
        virtual void warm_up_pipelines(int /*samples*/) {}
        /** Cumulative number of pipeline states this backend has created (0 where the concept does not apply). */
        virtual std::uint64_t pipelines_created() const { return 0; }
        /** True when the backend can record queued mesh draws on several threads (Vulkan, Direct3D 12). */
        virtual bool supports_parallel_recording() const { return false; }
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
        /** Restrict subsequent 2D drawing to a rectangle in drawing coordinates, or remove the restriction. */
        virtual void set_clip_rect_2d(const ClipRect2D &) {}
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
    std::unique_ptr<Renderer> create_renderer(GraphicsBackend backend);
    /** Return the active renderer, or nullptr before display initialization. */
    Renderer *active_renderer();
    /** Make the per-frame instance buffers used by draw_mesh_instanced(vector) available for reuse. Called once per frame. */
    void recycle_transient_instance_buffers();
    /** Destroy those buffers; called before the renderer shuts down. */
    void release_transient_instance_buffers();
}
