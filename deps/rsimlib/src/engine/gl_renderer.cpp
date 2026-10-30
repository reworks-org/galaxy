/** @file
 * @brief Implements the OpenGL 4.3 Renderer backend.
 */

#include "gl_renderer.h"
#include "gl_context.h"
#include "shader_paths.h"

#include <glad/glad.h>

#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <unordered_set>

namespace sl::detail
{
    namespace
    {
        std::string load_glsl_shader(const char *name)
        {
            const std::filesystem::path relative = std::filesystem::path("glsl") / name;
            std::ifstream file(find_shader_root(relative) / relative);
            return file ? std::string(std::istreambuf_iterator<char>(file), {}) : std::string{};
        }

        std::string shader_log(GLuint shader)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            if (length <= 1)
                return "Shader compilation failed.";
            std::vector<GLchar> log(static_cast<std::size_t>(length));
            glGetShaderInfoLog(shader, length, nullptr, log.data());
            return log.data();
        }

        std::string program_log(GLuint program)
        {
            GLint length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            if (length <= 1)
                return "Shader linking failed.";
            std::vector<GLchar> log(static_cast<std::size_t>(length));
            glGetProgramInfoLog(program, length, nullptr, log.data());
            return log.data();
        }

        GLuint compile_shader(GLenum type, const std::string &source, std::string &error)
        {
            const GLuint shader = glCreateShader(type);
            if (shader == 0)
            {
                error = "Unable to create shader.";
                return 0;
            }
            const char *source_text = source.c_str();
            glShaderSource(shader, 1, &source_text, nullptr);
            glCompileShader(shader);
            GLint compiled = GL_FALSE;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_TRUE)
                return shader;
            error = shader_log(shader);
            glDeleteShader(shader);
            return 0;
        }

        GLuint link_program(GLuint first, GLuint second, std::string &error)
        {
            const GLuint program = glCreateProgram();
            if (program == 0)
            {
                error = "Unable to create shader program.";
                return 0;
            }
            glAttachShader(program, first);
            if (second != 0)
                glAttachShader(program, second);
            glLinkProgram(program);
            GLint linked = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linked);
            if (linked != GL_TRUE)
            {
                error = program_log(program);
                glDeleteProgram(program);
                return 0;
            }
            return program;
        }

        class GLRenderer final : public Renderer
        {
        public:
            bool initialise(SDL_Window *window, std::string &error) override
            {
                return context_.initialise(window, error);
            }

            void shutdown() override
            {
                shutdown_2d();
                while (!meshes_.empty())
                    destroy_mesh(meshes_.begin()->first);
                if (mesh_program_ != 0)
                {
                    glDeleteProgram(mesh_program_);
                    mesh_program_ = 0;
                }
                if (mesh_instanced_program_ != 0)
                {
                    glDeleteProgram(mesh_instanced_program_);
                    mesh_instanced_program_ = 0;
                }
                while (!instance_buffers_.empty())
                    destroy_instance_buffer(instance_buffers_.begin()->first);
                if (terrain_program_ != 0)
                {
                    glDeleteProgram(terrain_program_);
                    terrain_program_ = 0;
                }
                if (sky_program_ != 0)
                {
                    glDeleteProgram(sky_program_);
                    sky_program_ = 0;
                }
                if (sky_vao_ != 0)
                {
                    glDeleteVertexArrays(1, &sky_vao_);
                    sky_vao_ = 0;
                }
                depth_buffers_.clear();
                multisample_targets_.clear();
                texture_sizes_.clear();
                mipmapped_textures_.clear();
                render_target_stack_.clear();
                context_.shutdown();
            }

            bool resize(int width, int height, std::string &error) override
            {
                return context_.resize(width, height, error);
            }

            void present() override
            {
                flush_2d();
                context_.present();
            }

            void wait_idle() override
            {
                flush_2d();
                glFinish();
            }

            bool begin_frame(std::string &) override { return true; }
            bool end_frame(std::string &) override
            {
                present();
                return true;
            }

            SDL_GLContext native_context() const override
            {
                return context_.native_context();
            }

            bool create_texture(const TextureDesc &description, std::uint32_t &texture) override
            {
                texture = 0;
                if (description.width <= 0 || description.height <= 0 || !std::isfinite(description.anisotropy))
                {
                    return false;
                }
                GLuint handle = 0;
                glGenTextures(1, &handle);
                if (handle == 0)
                {
                    return false;
                }
                GLint previous_active_texture = GL_TEXTURE0;
                GLint previous_texture = 0;
                GLint previous_unpack_buffer = 0;
                glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_active_texture);
                glActiveTexture(GL_TEXTURE0);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);
                glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &previous_unpack_buffer);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
                glBindTexture(GL_TEXTURE_2D, handle);
                const GLint filter = description.filter == TextureFilter::linear ? GL_LINEAR : GL_NEAREST;
                const GLint min_filter = !description.mipmaps ? filter
                                         : description.filter == TextureFilter::linear ? GL_LINEAR_MIPMAP_LINEAR
                                                                                       : GL_NEAREST_MIPMAP_NEAREST;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if (description.anisotropy > 1.0f &&
                    SDL_GL_ExtensionSupported("GL_EXT_texture_filter_anisotropic"))
                {
                    GLfloat max_anisotropy = 1.0f;
                    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
                    if (glGetError() == GL_NO_ERROR && max_anisotropy > 1.0f)
                    {
                        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                                        std::clamp(description.anisotropy, 1.0f, max_anisotropy));
                    }
                }
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, description.width, description.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                if (description.mipmaps)
                {
                    // Allocate every level now so the texture is complete before its first upload.
                    const int levels = mip_level_count(description.width, description.height);
                    for (int level = 1; level < levels; ++level)
                        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA8, std::max(description.width >> level, 1),
                                     std::max(description.height >> level, 1), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                }
                const bool created = glGetError() == GL_NO_ERROR;
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture));
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, static_cast<GLuint>(previous_unpack_buffer));
                glActiveTexture(static_cast<GLenum>(previous_active_texture));
                if (!created)
                {
                    glDeleteTextures(1, &handle);
                    return false;
                }
                texture_sizes_[handle] = {description.width, description.height};
                if (description.mipmaps)
                    mipmapped_textures_.insert(handle);
                texture = handle;
                return true;
            }

            static GLenum compressed_gl_format(CompressedFormat format)
            {
                switch (format)
                {
                case CompressedFormat::bc1:
                    return 0x83F1; // GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
                case CompressedFormat::bc3:
                    return 0x83F3; // GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
                default:
                    return 0x8E8C; // GL_COMPRESSED_RGBA_BPTC_UNORM
                }
            }

            bool supports_compressed_format(CompressedFormat format) const override
            {
                if (format == CompressedFormat::bc7)
                    return SDL_GL_ExtensionSupported("GL_ARB_texture_compression_bptc");
                return SDL_GL_ExtensionSupported("GL_EXT_texture_compression_s3tc");
            }

            bool create_texture_compressed(const CompressedTextureDesc &description, const std::uint8_t *data,
                                           std::uint32_t &texture) override
            {
                texture = 0;
                if (!data || compressed_texture_byte_count(description) == 0 || !supports_compressed_format(description.format))
                    return false;
                GLuint handle = 0;
                glGenTextures(1, &handle);
                if (handle == 0)
                    return false;
                GLint previous_active_texture = GL_TEXTURE0;
                GLint previous_texture = 0;
                GLint previous_unpack_buffer = 0;
                glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_active_texture);
                glActiveTexture(GL_TEXTURE0);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);
                glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &previous_unpack_buffer);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
                glBindTexture(GL_TEXTURE_2D, handle);
                const bool mipmapped = description.mip_levels > 1;
                const GLint filter = description.filter == TextureFilter::linear ? GL_LINEAR : GL_NEAREST;
                const GLint min_filter = !mipmapped ? filter
                                         : description.filter == TextureFilter::linear ? GL_LINEAR_MIPMAP_LINEAR
                                                                                       : GL_NEAREST_MIPMAP_NEAREST;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, description.mip_levels - 1);
                if (description.anisotropy > 1.0f && SDL_GL_ExtensionSupported("GL_EXT_texture_filter_anisotropic"))
                {
                    GLfloat max_anisotropy = 1.0f;
                    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
                    if (glGetError() == GL_NO_ERROR && max_anisotropy > 1.0f)
                        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                                        std::clamp(description.anisotropy, 1.0f, max_anisotropy));
                }
                std::size_t offset = 0;
                for (int level = 0; level < description.mip_levels; ++level)
                {
                    const int width = std::max(description.width >> level, 1);
                    const int height = std::max(description.height >> level, 1);
                    const std::size_t bytes = compressed_level_byte_count(description.format, width, height);
                    glCompressedTexImage2D(GL_TEXTURE_2D, level, compressed_gl_format(description.format), width, height, 0,
                                           static_cast<GLsizei>(bytes), data + offset);
                    offset += bytes;
                }
                const bool created = glGetError() == GL_NO_ERROR;
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture));
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, static_cast<GLuint>(previous_unpack_buffer));
                glActiveTexture(static_cast<GLenum>(previous_active_texture));
                if (!created)
                {
                    glDeleteTextures(1, &handle);
                    return false;
                }
                texture_sizes_[handle] = {description.width, description.height};
                texture = handle;
                return true;
            }

            bool upload_texture(std::uint32_t texture, int width, int height,
                                const std::uint8_t *pixels) override
            {
                if (texture == 0 || !pixels || width <= 0 || height <= 0)
                {
                    return false;
                }
                const GLuint handle = static_cast<GLuint>(texture);
                const auto size = texture_sizes_.find(handle);
                if (size == texture_sizes_.end() || width > size->second.first || height > size->second.second)
                {
                    return false;
                }
                GLint previous_active_texture = GL_TEXTURE0;
                GLint previous_texture = 0;
                GLint previous_unpack_alignment = 4;
                GLint previous_unpack_row_length = 0;
                GLint previous_unpack_skip_rows = 0;
                GLint previous_unpack_skip_pixels = 0;
                GLint previous_unpack_buffer = 0;
                glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_active_texture);
                glActiveTexture(GL_TEXTURE0);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);
                glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_unpack_alignment);
                glGetIntegerv(GL_UNPACK_ROW_LENGTH, &previous_unpack_row_length);
                glGetIntegerv(GL_UNPACK_SKIP_ROWS, &previous_unpack_skip_rows);
                glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &previous_unpack_skip_pixels);
                glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &previous_unpack_buffer);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
                glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
                glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
                glBindTexture(GL_TEXTURE_2D, handle);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                                GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                if (mipmapped_textures_.count(handle) != 0)
                    glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture));
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, static_cast<GLuint>(previous_unpack_buffer));
                glPixelStorei(GL_UNPACK_ALIGNMENT, previous_unpack_alignment);
                glPixelStorei(GL_UNPACK_ROW_LENGTH, previous_unpack_row_length);
                glPixelStorei(GL_UNPACK_SKIP_ROWS, previous_unpack_skip_rows);
                glPixelStorei(GL_UNPACK_SKIP_PIXELS, previous_unpack_skip_pixels);
                glActiveTexture(static_cast<GLenum>(previous_active_texture));
                return true;
            }

            bool download_texture(std::uint32_t texture, int width, int height,
                                  std::uint8_t *out_pixels) override
            {
                if (texture == 0 || !out_pixels || width <= 0 || height <= 0)
                {
                    return false;
                }
                const auto size = texture_sizes_.find(static_cast<GLuint>(texture));
                if (size == texture_sizes_.end() || width != size->second.first || height != size->second.second)
                    return false;
                flush_2d();
                GLint previous_active_texture = GL_TEXTURE0;
                GLint previous_texture = 0;
                GLint previous_pack_alignment = 4;
                GLint previous_pack_row_length = 0;
                GLint previous_pack_skip_rows = 0;
                GLint previous_pack_skip_pixels = 0;
                GLint previous_pack_buffer = 0;
                glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_active_texture);
                glActiveTexture(GL_TEXTURE0);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);
                glGetIntegerv(GL_PACK_ALIGNMENT, &previous_pack_alignment);
                glGetIntegerv(GL_PACK_ROW_LENGTH, &previous_pack_row_length);
                glGetIntegerv(GL_PACK_SKIP_ROWS, &previous_pack_skip_rows);
                glGetIntegerv(GL_PACK_SKIP_PIXELS, &previous_pack_skip_pixels);
                glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &previous_pack_buffer);
                glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glPixelStorei(GL_PACK_ROW_LENGTH, 0);
                glPixelStorei(GL_PACK_SKIP_ROWS, 0);
                glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texture));
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, out_pixels);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture));
                glBindBuffer(GL_PIXEL_PACK_BUFFER, static_cast<GLuint>(previous_pack_buffer));
                glPixelStorei(GL_PACK_ALIGNMENT, previous_pack_alignment);
                glPixelStorei(GL_PACK_ROW_LENGTH, previous_pack_row_length);
                glPixelStorei(GL_PACK_SKIP_ROWS, previous_pack_skip_rows);
                glPixelStorei(GL_PACK_SKIP_PIXELS, previous_pack_skip_pixels);
                glActiveTexture(static_cast<GLenum>(previous_active_texture));
                return true;
            }

            void destroy_texture(std::uint32_t texture) override
            {
                if (texture != 0)
                {
                    if (batch_texture_ == texture)
                    {
                        flush_2d();
                    }
                    const GLuint handle = static_cast<GLuint>(texture);
                    glDeleteTextures(1, &handle);
                    texture_sizes_.erase(handle);
                    mipmapped_textures_.erase(handle);
                }
            }

            int max_texture_size() const override
            {
                GLint size = 0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
                return size > 0 ? size : 16384;
            }

            int max_texture_array_layers() const override
            {
                GLint layers = 0;
                glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layers);
                return std::min<GLint>(layers, 2048);
            }

            bool create_texture_array(const TextureArrayDesc &description, const std::uint8_t *pixels,
                                      std::uint32_t &texture) override
            {
                texture = 0;
                GLint max_size = 0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
                if (!pixels || description.width <= 0 || description.height <= 0 || description.layers <= 0 ||
                    description.layers > max_texture_array_layers() || description.mip_levels <= 0 ||
                    description.width > max_size || description.height > max_size)
                    return false;
                GLuint handle = 0;
                glGenTextures(1, &handle);
                if (handle == 0)
                    return false;
                GLint previous_active_texture = GL_TEXTURE0;
                GLint previous_texture = 0;
                GLint previous_unpack_buffer = 0;
                GLint previous_unpack_alignment = 4;
                glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_active_texture);
                glActiveTexture(GL_TEXTURE0);
                glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &previous_texture);
                glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &previous_unpack_buffer);
                glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_unpack_alignment);
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                while (glGetError() != GL_NO_ERROR)
                {
                }
                glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, description.mip_levels - 1);
                for (int level = 0; level < description.mip_levels; ++level)
                    glTexImage3D(GL_TEXTURE_2D_ARRAY, level, GL_RGBA8, std::max(description.width >> level, 1),
                                 std::max(description.height >> level, 1), description.layers, 0, GL_RGBA,
                                 GL_UNSIGNED_BYTE, pixels + texture_array_level_offset(description, level));
                const bool created = glGetError() == GL_NO_ERROR;
                if (created)
                {
                    GLfloat anisotropy = 1.0f;
                    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
                    if (glGetError() == GL_NO_ERROR && anisotropy > 1.0f)
                        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(anisotropy, 16.0f));
                    while (glGetError() != GL_NO_ERROR)
                    {
                    }
                }
                glPixelStorei(GL_UNPACK_ALIGNMENT, previous_unpack_alignment);
                glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(previous_texture));
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, static_cast<GLuint>(previous_unpack_buffer));
                glActiveTexture(static_cast<GLenum>(previous_active_texture));
                if (!created)
                {
                    glDeleteTextures(1, &handle);
                    return false;
                }
                texture = handle;
                return true;
            }

            void destroy_texture_array(std::uint32_t texture) override
            {
                const GLuint handle = static_cast<GLuint>(texture);
                if (handle != 0)
                    glDeleteTextures(1, &handle);
            }

            bool create_render_target(int width, int height, std::uint32_t &texture,
                                      std::uint32_t &framebuffer) override
            {
                texture = 0;
                framebuffer = 0;
                GLint previous_draw_framebuffer = 0;
                GLint previous_read_framebuffer = 0;
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_draw_framebuffer);
                glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous_read_framebuffer);
                const auto restore_state = [&]()
                {
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(previous_draw_framebuffer));
                    glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(previous_read_framebuffer));
                };
                if (width <= 0 || height <= 0)
                {
                    return false;
                }
                if (!create_texture({width, height, TextureFilter::linear}, texture))
                {
                    std::fprintf(stderr, "create_texture failed: GL error 0x%x\n", glGetError());
                    return false;
                }
                GLuint fbo = 0;
                glGenFramebuffers(1, &fbo);
                if (fbo == 0)
                {
                    destroy_texture(texture);
                    texture = 0;
                    restore_state();
                    return false;
                }
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                       static_cast<GLuint>(texture), 0);
                const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
                if (!complete)
                {
                    destroy_texture(texture);
                    texture = 0;
                    if (fbo != 0)
                    {
                        glDeleteFramebuffers(1, &fbo);
                    }
                    restore_state();
                    return false;
                }
                restore_state();
                framebuffer = fbo;
                return true;
            }

            void destroy_render_target(std::uint32_t texture, std::uint32_t framebuffer) override
            {
                flush_2d();
                if (framebuffer != 0)
                {
                    const GLuint handle = static_cast<GLuint>(framebuffer);
                    glDeleteFramebuffers(1, &handle);
                        const auto multisample = multisample_targets_.find(handle);
                    if (multisample != multisample_targets_.end())
                    {
                        glDeleteFramebuffers(1, &multisample->second.framebuffer);
                        glDeleteRenderbuffers(1, &multisample->second.color);
                        glDeleteRenderbuffers(1, &multisample->second.depth);
                        multisample_targets_.erase(multisample);
                    }
                    const auto depth = depth_buffers_.find(handle);
                    if (depth != depth_buffers_.end())
                    {
                        glDeleteRenderbuffers(1, &depth->second);
                        depth_buffers_.erase(depth);
                    }
                }
                destroy_texture(texture);
            }

            bool create_multisample_target(int width, int height, int samples, std::uint32_t texture,
                                           std::uint32_t framebuffer)
            {
                MultisampleTarget target;
                target.samples = samples;
                GLint previous_draw_framebuffer = 0;
                GLint previous_read_framebuffer = 0;
                GLint previous_renderbuffer = 0;
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_draw_framebuffer);
                glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous_read_framebuffer);
                glGetIntegerv(GL_RENDERBUFFER_BINDING, &previous_renderbuffer);
                glGenFramebuffers(1, &target.framebuffer);
                glGenRenderbuffers(1, &target.color);
                glGenRenderbuffers(1, &target.depth);
                if (target.framebuffer == 0 || target.color == 0 || target.depth == 0)
                {
                    glDeleteFramebuffers(1, &target.framebuffer);
                    glDeleteRenderbuffers(1, &target.color);
                    glDeleteRenderbuffers(1, &target.depth);
                    return false;
                }
                glBindRenderbuffer(GL_RENDERBUFFER, target.color);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
                glBindRenderbuffer(GL_RENDERBUFFER, target.depth);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
                glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, target.color);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, target.depth);
                const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(previous_draw_framebuffer));
                glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(previous_read_framebuffer));
                glBindRenderbuffer(GL_RENDERBUFFER, static_cast<GLuint>(previous_renderbuffer));
                if (!complete)
                {
                    glDeleteFramebuffers(1, &target.framebuffer);
                    glDeleteRenderbuffers(1, &target.color);
                    glDeleteRenderbuffers(1, &target.depth);
                    return false;
                }
                multisample_targets_[static_cast<GLuint>(framebuffer)] = target;
                return true;
            }

            bool create_render_target_3d(int width, int height, int &samples, std::uint32_t &texture,
                                         std::uint32_t &framebuffer) override
            {
                if (!create_render_target(width, height, texture, framebuffer))
                    return false;
                GLint max_samples = 1;
                glGetIntegerv(GL_MAX_SAMPLES, &max_samples);
                samples = samples > max_samples ? max_samples : samples;
                if (samples > 1)
                {
                    if (create_multisample_target(width, height, samples, texture, framebuffer))
                        return true;
                    samples = 1;
                }
                GLint previous_draw_framebuffer = 0;
                GLint previous_read_framebuffer = 0;
                GLint previous_renderbuffer = 0;
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_draw_framebuffer);
                glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous_read_framebuffer);
                glGetIntegerv(GL_RENDERBUFFER_BINDING, &previous_renderbuffer);
                GLuint depth = 0;
                glGenRenderbuffers(1, &depth);
                glBindRenderbuffer(GL_RENDERBUFFER, depth);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
                glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(framebuffer));
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
                const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(previous_draw_framebuffer));
                glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(previous_read_framebuffer));
                glBindRenderbuffer(GL_RENDERBUFFER, static_cast<GLuint>(previous_renderbuffer));
                if (!complete)
                {
                    glDeleteRenderbuffers(1, &depth);
                    destroy_render_target(texture, framebuffer);
                    texture = 0;
                    framebuffer = 0;
                    return false;
                }
                depth_buffers_[static_cast<GLuint>(framebuffer)] = depth;
                return true;
            }

            bool create_mesh(const Vertex3D *vertices, std::size_t vertex_count,
                             const std::uint32_t *indices, std::size_t index_count,
                             std::uint32_t &handle) override
            {
                handle = 0;
                if (!vertices || !indices || vertex_count == 0 || index_count == 0 ||
                    index_count > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max()) ||
                    vertex_count > static_cast<std::size_t>(std::numeric_limits<GLsizeiptr>::max()) / sizeof(Vertex3D) ||
                    index_count > static_cast<std::size_t>(std::numeric_limits<GLsizeiptr>::max()) / sizeof(std::uint32_t))
                    return false;
                GLMesh mesh;
                mesh.index_count = static_cast<GLsizei>(index_count);
                mesh.vertex_count = vertex_count;
                glGenVertexArrays(1, &mesh.vao);
                glGenBuffers(1, &mesh.vbo);
                glGenBuffers(1, &mesh.ebo);
                if (mesh.vao == 0 || mesh.vbo == 0 || mesh.ebo == 0)
                {
                    glDeleteVertexArrays(1, &mesh.vao);
                    glDeleteBuffers(1, &mesh.vbo);
                    glDeleteBuffers(1, &mesh.ebo);
                    return false;
                }
                GLint previous_vertex_array = 0;
                GLint previous_array_buffer = 0;
                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vertex_array);
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &previous_array_buffer);
                glBindVertexArray(mesh.vao);
                glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertex_count, vertices, GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * index_count, indices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                                      reinterpret_cast<void *>(offsetof(Vertex3D, position)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                                      reinterpret_cast<void *>(offsetof(Vertex3D, normal)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                                      reinterpret_cast<void *>(offsetof(Vertex3D, colour)));
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                                      reinterpret_cast<void *>(offsetof(Vertex3D, uv)));
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                                      reinterpret_cast<void *>(offsetof(Vertex3D, tangent)));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                                      reinterpret_cast<void *>(offsetof(Vertex3D, extra)));
                glBindVertexArray(static_cast<GLuint>(previous_vertex_array));
                glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(previous_array_buffer));
                handle = next_mesh_++;
                meshes_[handle] = mesh;
                return true;
            }

            bool update_mesh_vertices(std::uint32_t handle, const Vertex3D *vertices, std::size_t vertex_count) override
            {
                const auto found = meshes_.find(handle);
                if (found == meshes_.end() || !vertices || vertex_count != found->second.vertex_count)
                    return false;
                GLint previous_array_buffer = 0;
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &previous_array_buffer);
                glBindBuffer(GL_ARRAY_BUFFER, found->second.vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(Vertex3D) * vertex_count), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(previous_array_buffer));
                return true;
            }

            void destroy_mesh(std::uint32_t handle) override
            {
                const auto found = meshes_.find(handle);
                if (found == meshes_.end())
                    return;
                glDeleteVertexArrays(1, &found->second.vao);
                glDeleteBuffers(1, &found->second.vbo);
                glDeleteBuffers(1, &found->second.ebo);
                meshes_.erase(found);
            }

            bool clear_3d(float red, float green, float blue, float alpha) override
            {
                flush_2d();
                GLfloat previous_clear_color[4] = {};
                GLdouble previous_clear_depth = 1.0;
                GLboolean previous_color_mask[4] = {};
                GLboolean previous_depth_mask = GL_TRUE;
                const GLboolean scissor_was_enabled = glIsEnabled(GL_SCISSOR_TEST);
                glGetFloatv(GL_COLOR_CLEAR_VALUE, previous_clear_color);
                glGetDoublev(GL_DEPTH_CLEAR_VALUE, &previous_clear_depth);
                glGetBooleanv(GL_COLOR_WRITEMASK, previous_color_mask);
                glGetBooleanv(GL_DEPTH_WRITEMASK, &previous_depth_mask);
                glDisable(GL_SCISSOR_TEST);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                glDepthMask(GL_TRUE);
                glClearColor(red, green, blue, alpha);
                glClearDepth(1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(previous_clear_color[0], previous_clear_color[1], previous_clear_color[2], previous_clear_color[3]);
                glClearDepth(previous_clear_depth);
                glColorMask(previous_color_mask[0], previous_color_mask[1], previous_color_mask[2], previous_color_mask[3]);
                glDepthMask(previous_depth_mask);
                if (scissor_was_enabled)
                    glEnable(GL_SCISSOR_TEST);
                return true;
            }

            bool clear_depth_3d() override
            {
                flush_2d();
                GLdouble previous_clear_depth = 1.0;
                GLboolean previous_depth_mask = GL_TRUE;
                const GLboolean scissor_was_enabled = glIsEnabled(GL_SCISSOR_TEST);
                glGetDoublev(GL_DEPTH_CLEAR_VALUE, &previous_clear_depth);
                glGetBooleanv(GL_DEPTH_WRITEMASK, &previous_depth_mask);
                glDisable(GL_SCISSOR_TEST);
                glDepthMask(GL_TRUE);
                glClearDepth(1.0);
                glClear(GL_DEPTH_BUFFER_BIT);
                glClearDepth(previous_clear_depth);
                glDepthMask(previous_depth_mask);
                if (scissor_was_enabled)
                    glEnable(GL_SCISSOR_TEST);
                return true;
            }

            struct GLInstanceBuffer
            {
                GLuint buffer = 0;
                std::size_t capacity = 0;
            };
            bool draw_mesh(std::uint32_t handle, const MeshDrawParams &params) override
            {
                if (params.terrain_albedo != 0)
                {
                    if (meshes_.find(handle) == meshes_.end() || !initialise_2d())
                        return false;
                    return draw_terrain(handle, params);
                }
                return draw_mesh_common(handle, params, nullptr, 0);
            }

            bool create_instance_buffer(std::size_t capacity, std::uint32_t &handle) override
            {
                handle = 0;
                GLuint buffer = 0;
                glGenBuffers(1, &buffer);
                if (buffer == 0)
                    return false;
                GLint previous_buffer = 0;
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &previous_buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(InstanceGpu) * capacity), nullptr,
                             GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(previous_buffer));
                handle = next_instance_buffer_++;
                instance_buffers_[handle] = {buffer, capacity};
                return true;
            }

            bool update_instance_buffer(std::uint32_t handle, const InstanceGpu *instances, std::size_t count) override
            {
                const auto found = instance_buffers_.find(handle);
                if (found == instance_buffers_.end() || count > found->second.capacity || (!instances && count > 0))
                    return false;
                GLint previous_buffer = 0;
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &previous_buffer);
                glBindBuffer(GL_ARRAY_BUFFER, found->second.buffer);
                // Orphan the storage so an in-flight draw using the old contents is not stalled.
                glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(InstanceGpu) * found->second.capacity),
                             nullptr, GL_DYNAMIC_DRAW);
                if (count > 0)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(InstanceGpu) * count), instances);
                glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(previous_buffer));
                return true;
            }

            void destroy_instance_buffer(std::uint32_t handle) override
            {
                const auto found = instance_buffers_.find(handle);
                if (found == instance_buffers_.end())
                    return;
                glDeleteBuffers(1, &found->second.buffer);
                instance_buffers_.erase(found);
            }

            bool draw_mesh_instanced(std::uint32_t handle, const MeshDrawParams &params, std::uint32_t instance_buffer,
                                     std::size_t count) override
            {
                const auto instances = instance_buffers_.find(instance_buffer);
                if (instances == instance_buffers_.end() || count == 0 || count > instances->second.capacity ||
                    params.terrain_albedo != 0)
                    return false;
                return draw_mesh_common(handle, params, &instances->second, count);
            }

            bool draw_mesh_common(std::uint32_t handle, const MeshDrawParams &params,
                                  const GLInstanceBuffer *instances, std::size_t instance_count)
            {
                const auto found = meshes_.find(handle);
                if (found == meshes_.end() || !initialise_2d())
                    return false;
                std::uint32_t &program = instances ? mesh_instanced_program_ : mesh_program_;
                if (program == 0)
                {
                    std::string error;
                    if (!create_shader(ShaderSource::glsl(load_glsl_shader(instances ? "mesh3d_instanced.vert" : "mesh3d.vert")),
                                       ShaderSource::glsl(load_glsl_shader("mesh3d.frag")),
                                       program, error))
                        return false;
                }
                flush_2d();
                GLint previous_program = 0;
                GLint previous_depth_func = GL_LESS;
                GLint previous_depth_mask = GL_TRUE;
                GLint previous_cull_mode = GL_BACK;
                GLint previous_front_face = GL_CCW;
                GLint previous_active_texture = GL_TEXTURE0;
                GLint previous_vertex_array = 0;
                GLint previous_texture0 = 0;
                GLint previous_texture1 = 0;
                GLint previous_texture2 = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
                glGetIntegerv(GL_DEPTH_FUNC, &previous_depth_func);
                glGetIntegerv(GL_DEPTH_WRITEMASK, &previous_depth_mask);
                glGetIntegerv(GL_CULL_FACE_MODE, &previous_cull_mode);
                glGetIntegerv(GL_FRONT_FACE, &previous_front_face);
                glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_active_texture);
                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vertex_array);
                glActiveTexture(GL_TEXTURE0);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture0);
                glActiveTexture(GL_TEXTURE1);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture1);
                glActiveTexture(GL_TEXTURE2);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture2);
                const GLboolean depth_was_enabled = glIsEnabled(GL_DEPTH_TEST);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glDepthMask(params.blend == 0 ? GL_TRUE : GL_FALSE);
                const GLboolean blend_was_enabled = glIsEnabled(GL_BLEND);
                GLint previous_blend_src_rgb = GL_SRC_ALPHA, previous_blend_dst_rgb = GL_ONE_MINUS_SRC_ALPHA;
                GLint previous_blend_src_alpha = GL_ONE, previous_blend_dst_alpha = GL_ONE_MINUS_SRC_ALPHA;
                glGetIntegerv(GL_BLEND_SRC_RGB, &previous_blend_src_rgb);
                glGetIntegerv(GL_BLEND_DST_RGB, &previous_blend_dst_rgb);
                glGetIntegerv(GL_BLEND_SRC_ALPHA, &previous_blend_src_alpha);
                glGetIntegerv(GL_BLEND_DST_ALPHA, &previous_blend_dst_alpha);
                if (params.blend == 0)
                {
                    glDisable(GL_BLEND);
                }
                else
                {
                    glEnable(GL_BLEND);
                    glBlendFuncSeparate(GL_SRC_ALPHA, params.blend == 2 ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
                                        GL_ONE_MINUS_SRC_ALPHA);
                }
                const GLboolean cull_was_enabled = glIsEnabled(GL_CULL_FACE);
                if (params.cull == CullMode::none)
                {
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(params.cull == CullMode::front ? GL_FRONT : GL_BACK);
                    glFrontFace(GL_CCW);
                }
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.texture != 0 ? params.texture : white_texture_));
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.normal_texture != 0 ? params.normal_texture : white_texture_));
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.material_texture != 0 ? params.material_texture : white_texture_));
                GLint previous_texture3 = 0;
                glActiveTexture(GL_TEXTURE3);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture3);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.shadow_texture != 0 ? params.shadow_texture : white_texture_));
                GLint previous_texture4 = 0;
                glActiveTexture(GL_TEXTURE4);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture4);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.local_shadow_texture != 0 ? params.local_shadow_texture : white_texture_));
                glActiveTexture(GL_TEXTURE0);
                glUseProgram(program);
                glUniform1i(glGetUniformLocation(program, "uTexture"), 0);
                glUniform1i(glGetUniformLocation(program, "uNormalMap"), 1);
                glUniform1i(glGetUniformLocation(program, "uMaterial"), 2);
                glUniform1i(glGetUniformLocation(program, "uShadowMap"), 3);
                glUniform4fv(glGetUniformLocation(program, "uShadow"), 1, params.shadow);
                glUniformMatrix4fv(glGetUniformLocation(program, "uShadowMatrix"), 1, GL_FALSE, params.shadow_matrix);
                glUniform4fv(glGetUniformLocation(program, "uPbr"), 1, params.pbr);
                glUniform1f(glGetUniformLocation(program, "uStyle"), static_cast<float>(params.style));
                if (params.style == 2)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glUniform4fv(glGetUniformLocation(program, "uEmissive"), 1, params.emissive);
                glUniform4fv(glGetUniformLocation(program, "uCameraRight"), 1, params.camera_right);
                glUniform4fv(glGetUniformLocation(program, "uCameraUp"), 1, params.camera_up);
                glUniform4fv(glGetUniformLocation(program, "uCameraForward"), 1, params.camera_forward);
                glUniform4fv(glGetUniformLocation(program, "uCameraPosition"), 1, params.camera_position);
                if (params.camera_position[3] > 0.5f)
                {
                    glUniform4fv(glGetUniformLocation(program, "uLocals"), static_cast<GLsizei>(params.camera_position[3]) * 4,
                                 &params.local_lights[0][0]);
                    if (params.shadow[3] > 0.5f)
                    {
                        glUniform4fv(glGetUniformLocation(program, "uLocals[64]"),
                                     static_cast<GLsizei>(params.shadow[3]) * 24, &params.local_lights[local_shadow_matrix_base][0]);
                    }
                }
                glUniform1i(glGetUniformLocation(program, "uLocalShadowMap"), 4);
                glUniformMatrix4fv(glGetUniformLocation(program, "uMvp"), 1, GL_FALSE, params.mvp);
                glUniform4fv(glGetUniformLocation(program, "uNormalRows"), 3, &params.normal_rows[0][0]);
                glUniform4fv(glGetUniformLocation(program, "uLight"), 1, params.light);
                glUniform4fv(glGetUniformLocation(program, "uLightColour"), 1, params.light_colour);
                glUniform4fv(glGetUniformLocation(program, "uFogColour"), 1, params.fog_colour);
                glUniform4fv(glGetUniformLocation(program, "uFogBeta"), 1, params.fog_beta);
                glUniform4fv(glGetUniformLocation(program, "uFogView"), 1, params.fog_view);
                glBindVertexArray(found->second.vao);
                if (instances)
                {
                    GLint previous_array_buffer = 0;
                    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &previous_array_buffer);
                    glBindBuffer(GL_ARRAY_BUFFER, instances->buffer);
                    // Locations 6-9 are the model matrix columns, 10 the tint; all advance once per instance.
                    for (GLuint attribute = 0; attribute < 5; ++attribute)
                    {
                        glEnableVertexAttribArray(6 + attribute);
                        glVertexAttribPointer(6 + attribute, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceGpu),
                                              reinterpret_cast<void *>(sizeof(float) * 4 * attribute));
                        glVertexAttribDivisor(6 + attribute, 1);
                    }
                    glDrawElementsInstanced(GL_TRIANGLES, found->second.index_count, GL_UNSIGNED_INT, nullptr,
                                            static_cast<GLsizei>(instance_count));
                    for (GLuint attribute = 0; attribute < 5; ++attribute)
                    {
                        glVertexAttribDivisor(6 + attribute, 0);
                        glDisableVertexAttribArray(6 + attribute);
                    }
                    glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(previous_array_buffer));
                }
                else
                {
                    glDrawElements(GL_TRIANGLES, found->second.index_count, GL_UNSIGNED_INT, nullptr);
                }
                if (params.style == 2)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glBindVertexArray(static_cast<GLuint>(previous_vertex_array));
                glUseProgram(static_cast<GLuint>(previous_program));
                glDepthFunc(static_cast<GLenum>(previous_depth_func));
                glDepthMask(static_cast<GLboolean>(previous_depth_mask));
                glBlendFuncSeparate(static_cast<GLenum>(previous_blend_src_rgb), static_cast<GLenum>(previous_blend_dst_rgb),
                                    static_cast<GLenum>(previous_blend_src_alpha), static_cast<GLenum>(previous_blend_dst_alpha));
                if (blend_was_enabled)
                    glEnable(GL_BLEND);
                else
                    glDisable(GL_BLEND);
                glCullFace(static_cast<GLenum>(previous_cull_mode));
                glFrontFace(static_cast<GLenum>(previous_front_face));
                if (cull_was_enabled)
                    glEnable(GL_CULL_FACE);
                else
                    glDisable(GL_CULL_FACE);
                if (depth_was_enabled)
                    glEnable(GL_DEPTH_TEST);
                else
                    glDisable(GL_DEPTH_TEST);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture0));
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture1));
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture2));
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture3));
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture4));
                glActiveTexture(static_cast<GLenum>(previous_active_texture));
                return true;
            }
            bool draw_terrain(std::uint32_t handle, const MeshDrawParams &params)
            {
                const auto found = meshes_.find(handle);
                if (found == meshes_.end())
                    return false;
                const auto &mesh = found->second;
                if (terrain_program_ == 0)
                {
                    std::string error;
                    if (!create_shader(ShaderSource::glsl(load_glsl_shader("terrain.vert")),
                                       ShaderSource::glsl(load_glsl_shader("terrain.frag")),
                                       terrain_program_, error))
                        return false;
                }
                flush_2d();
                GLint previous_program = 0;
                GLint previous_depth_func = GL_LESS;
                GLint previous_depth_mask = GL_TRUE;
                GLint previous_cull_mode = GL_BACK;
                GLint previous_front_face = GL_CCW;
                GLint previous_active_texture = GL_TEXTURE0;
                GLint previous_vertex_array = 0;
                GLint previous_array0 = 0;
                GLint previous_array1 = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
                glGetIntegerv(GL_DEPTH_FUNC, &previous_depth_func);
                glGetIntegerv(GL_DEPTH_WRITEMASK, &previous_depth_mask);
                glGetIntegerv(GL_CULL_FACE_MODE, &previous_cull_mode);
                glGetIntegerv(GL_FRONT_FACE, &previous_front_face);
                glGetIntegerv(GL_ACTIVE_TEXTURE, &previous_active_texture);
                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vertex_array);
                glActiveTexture(GL_TEXTURE0);
                glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &previous_array0);
                glActiveTexture(GL_TEXTURE1);
                glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &previous_array1);
                const GLboolean depth_was_enabled = glIsEnabled(GL_DEPTH_TEST);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glDepthMask(GL_TRUE);
                const GLboolean cull_was_enabled = glIsEnabled(GL_CULL_FACE);
                if (params.cull == CullMode::none)
                {
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(params.cull == CullMode::front ? GL_FRONT : GL_BACK);
                    glFrontFace(GL_CCW);
                }
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(params.terrain_albedo));
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(params.terrain_normals != 0 ? params.terrain_normals : params.terrain_albedo));
                GLint previous_texture2 = 0;
                GLint previous_texture3 = 0;
                glActiveTexture(GL_TEXTURE2);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture2);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.shadow_texture != 0 ? params.shadow_texture : white_texture_));
                glActiveTexture(GL_TEXTURE3);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture3);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.local_shadow_texture != 0 ? params.local_shadow_texture : white_texture_));
                glActiveTexture(GL_TEXTURE0);
                glUseProgram(terrain_program_);
                glUniform1i(glGetUniformLocation(terrain_program_, "uShadowMap"), 2);
                glUniform1i(glGetUniformLocation(terrain_program_, "uLocalShadowMap"), 3);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uShadow"), 1, params.shadow);
                glUniformMatrix4fv(glGetUniformLocation(terrain_program_, "uShadowMatrix"), 1, GL_FALSE, params.shadow_matrix);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uCameraPosition"), 1, params.camera_position);
                if (params.camera_position[3] > 0.5f)
                {
                    glUniform4fv(glGetUniformLocation(terrain_program_, "uLocals"), static_cast<GLsizei>(params.camera_position[3]) * 4,
                                 &params.local_lights[0][0]);
                    if (params.shadow[3] > 0.5f)
                        glUniform4fv(glGetUniformLocation(terrain_program_, "uLocals[64]"),
                                     static_cast<GLsizei>(params.shadow[3]) * 24, &params.local_lights[local_shadow_matrix_base][0]);
                }
                float macro[4] = {params.terrain_macro[0], params.terrain_macro[1], params.terrain_macro[2],
                                  params.terrain_normals != 0 ? 1.0f : 0.0f};
                glUniform1i(glGetUniformLocation(terrain_program_, "uAlbedo"), 0);
                glUniform1i(glGetUniformLocation(terrain_program_, "uNormals"), 1);
                glUniformMatrix4fv(glGetUniformLocation(terrain_program_, "uMvp"), 1, GL_FALSE, params.mvp);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uTerrain"), 2, &params.normal_rows[1][0]);
                glUniform4f(glGetUniformLocation(terrain_program_, "uTurn"), params.normal_rows[0][0], params.normal_rows[0][1],
                            params.normal_rows[0][2], params.normal_rows[2][3]);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uLayers"), terrain_layer_count, &params.terrain_layers[0][0]);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uAverage"), terrain_layer_count, &params.terrain_average[0][0]);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uMacro"), 1, macro);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uPbr"), 1, params.pbr);
                glUniform1f(glGetUniformLocation(terrain_program_, "uStyle"), static_cast<float>(params.style));
                if (params.style == 2)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uLight"), 1, params.light);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uLightColour"), 1, params.light_colour);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uFogColour"), 1, params.fog_colour);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uFogBeta"), 1, params.fog_beta);
                glUniform4fv(glGetUniformLocation(terrain_program_, "uFogView"), 1, params.fog_view);
                glBindVertexArray(mesh.vao);
                glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr);
                if (params.style == 2)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glBindVertexArray(static_cast<GLuint>(previous_vertex_array));
                glUseProgram(static_cast<GLuint>(previous_program));
                glDepthFunc(static_cast<GLenum>(previous_depth_func));
                glDepthMask(static_cast<GLboolean>(previous_depth_mask));
                glCullFace(static_cast<GLenum>(previous_cull_mode));
                glFrontFace(static_cast<GLenum>(previous_front_face));
                if (cull_was_enabled)
                    glEnable(GL_CULL_FACE);
                else
                    glDisable(GL_CULL_FACE);
                if (depth_was_enabled)
                    glEnable(GL_DEPTH_TEST);
                else
                    glDisable(GL_DEPTH_TEST);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(previous_array0));
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(previous_array1));
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture2));
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previous_texture3));
                glActiveTexture(static_cast<GLenum>(previous_active_texture));
                return true;
            }

            bool draw_sky(const SkyDrawParams &params) override
            {
                if (!initialise_2d())
                    return false;
                if (sky_program_ == 0)
                {
                    std::string error;
                    if (!create_shader(ShaderSource::glsl(load_glsl_shader("sky.vert")),
                                       ShaderSource::glsl(load_glsl_shader("sky.frag")), sky_program_, error))
                        return false;
                }
                if (sky_vao_ == 0)
                    glGenVertexArrays(1, &sky_vao_);
                flush_2d();
                GLint previous_program = 0;
                GLint previous_depth_func = GL_LESS;
                GLint previous_depth_mask = GL_TRUE;
                GLint previous_vertex_array = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
                glGetIntegerv(GL_DEPTH_FUNC, &previous_depth_func);
                glGetIntegerv(GL_DEPTH_WRITEMASK, &previous_depth_mask);
                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vertex_array);
                const GLboolean depth_was_enabled = glIsEnabled(GL_DEPTH_TEST);
                const GLboolean cull_was_enabled = glIsEnabled(GL_CULL_FACE);
                const GLboolean blend_was_enabled = glIsEnabled(GL_BLEND);
                // The triangle sits on the far plane, so it only covers pixels nothing has drawn to.
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glDepthMask(GL_FALSE);
                glDisable(GL_CULL_FACE);
                glDisable(GL_BLEND);
                glUseProgram(sky_program_);
                glUniform4fv(glGetUniformLocation(sky_program_, "uSky"), SkyDrawParams::rows, &params.data[0][0]);
                glBindVertexArray(sky_vao_);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glBindVertexArray(static_cast<GLuint>(previous_vertex_array));
                glUseProgram(static_cast<GLuint>(previous_program));
                glDepthFunc(static_cast<GLenum>(previous_depth_func));
                glDepthMask(static_cast<GLboolean>(previous_depth_mask));
                if (!depth_was_enabled)
                    glDisable(GL_DEPTH_TEST);
                if (cull_was_enabled)
                    glEnable(GL_CULL_FACE);
                if (blend_was_enabled)
                    glEnable(GL_BLEND);
                return true;
            }
            bool begin_render_target(std::uint32_t framebuffer, int width, int height, std::string &) override
            {
                if (framebuffer == 0 || width <= 0 || height <= 0)
                {
                    return false;
                }
                flush_2d();
                RenderTargetState previous{};
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous.draw_framebuffer);
                glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous.read_framebuffer);
                glGetIntegerv(GL_VIEWPORT, previous.viewport);
                GLuint draw_framebuffer = static_cast<GLuint>(framebuffer);
                const auto multisample = multisample_targets_.find(draw_framebuffer);
                if (multisample != multisample_targets_.end())
                {
                    previous.resolve_framebuffer = draw_framebuffer;
                    previous.width = width;
                    previous.height = height;
                    draw_framebuffer = multisample->second.framebuffer;
                }
                render_target_stack_.push_back(previous);
                glBindFramebuffer(GL_FRAMEBUFFER, draw_framebuffer);
                glViewport(0, 0, width, height);
                return true;
            }
            bool end_render_target(std::string &) override
            {
                flush_2d();
                if (render_target_stack_.empty())
                {
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    return true;
                }
                const RenderTargetState previous = render_target_stack_.back();
                render_target_stack_.pop_back();
                if (previous.resolve_framebuffer != 0)
                {
                    GLint source = 0;
                    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &source);
                    glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(source));
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, previous.resolve_framebuffer);
                    glBlitFramebuffer(0, 0, previous.width, previous.height, 0, 0, previous.width, previous.height,
                                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
                }
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(previous.draw_framebuffer));
                glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(previous.read_framebuffer));
                glViewport(previous.viewport[0], previous.viewport[1], previous.viewport[2], previous.viewport[3]);
                return true;
            }

            bool download_render_target(std::uint32_t framebuffer, int width, int height,
                                        std::uint8_t *out_pixels) override
            {
                if (!out_pixels || width <= 0 || height <= 0)
                {
                    return false;
                }
                flush_2d();
                GLint previous_fbo = 0;
                GLint previous_pack_alignment = 4;
                GLint previous_pack_row_length = 0;
                GLint previous_pack_skip_rows = 0;
                GLint previous_pack_skip_pixels = 0;
                GLint previous_pack_buffer = 0;
                glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous_fbo);
                glGetIntegerv(GL_PACK_ALIGNMENT, &previous_pack_alignment);
                glGetIntegerv(GL_PACK_ROW_LENGTH, &previous_pack_row_length);
                glGetIntegerv(GL_PACK_SKIP_ROWS, &previous_pack_skip_rows);
                glGetIntegerv(GL_PACK_SKIP_PIXELS, &previous_pack_skip_pixels);
                glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &previous_pack_buffer);
                glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glPixelStorei(GL_PACK_ROW_LENGTH, 0);
                glPixelStorei(GL_PACK_SKIP_ROWS, 0);
                glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
                glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(framebuffer));
                glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, out_pixels);
                glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(previous_fbo));
                glBindBuffer(GL_PIXEL_PACK_BUFFER, static_cast<GLuint>(previous_pack_buffer));
                glPixelStorei(GL_PACK_ALIGNMENT, previous_pack_alignment);
                glPixelStorei(GL_PACK_ROW_LENGTH, previous_pack_row_length);
                glPixelStorei(GL_PACK_SKIP_ROWS, previous_pack_skip_rows);
                glPixelStorei(GL_PACK_SKIP_PIXELS, previous_pack_skip_pixels);

                // Flip rows vertically so origin is top-left
                const std::size_t row_bytes = static_cast<std::size_t>(width) * 4;
                std::vector<std::uint8_t> row(row_bytes);
                for (int top = 0, bottom = height - 1; top < bottom; ++top, --bottom)
                {
                    std::memcpy(row.data(), out_pixels + static_cast<std::size_t>(top) * row_bytes, row_bytes);
                    std::memcpy(out_pixels + static_cast<std::size_t>(top) * row_bytes,
                                out_pixels + static_cast<std::size_t>(bottom) * row_bytes, row_bytes);
                    std::memcpy(out_pixels + static_cast<std::size_t>(bottom) * row_bytes,
                                row.data(), row_bytes);
                }
                return true;
            }

            bool initialise_2d() override
            {
                if (vao_ != 0)
                    return true;
                const std::string vertex = load_glsl_shader("default_2d.vert");
                const std::string fragment = load_glsl_shader("default_2d.frag");
                if (vertex.empty() || fragment.empty())
                {
                    shader_error_ = "Unable to load default OpenGL shader assets.";
                    return false;
                }
                if (!create_shader({ShaderLanguage::glsl, vertex}, {ShaderLanguage::glsl, fragment},
                                   default_2d_shader_, shader_error_))
                    return false;
                glGenVertexArrays(1, &vao_);
                glGenBuffers(1, &vbo_);
                if (vao_ == 0 || vbo_ == 0)
                {
                    shutdown_2d();
                    shader_error_ = "Unable to create OpenGL 2D vertex resources.";
                    return false;
                }
                glBindVertexArray(vao_);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, x)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, u)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, r)));
                glBindVertexArray(0);
                const unsigned char white_pixel[4] = {255, 255, 255, 255};
                if (!create_texture({1, 1, TextureFilter::nearest}, white_texture_) ||
                    !upload_texture(white_texture_, 1, 1, white_pixel))
                {
                    shutdown_2d();
                    shader_error_ = "Unable to create the OpenGL default texture.";
                    return false;
                }
                return true;
            }

            void shutdown_2d() override
            {
                flush_2d();
                batch_vertices_.clear();
                batch_vertices_.shrink_to_fit();
                if (vbo_ != 0)
                    glDeleteBuffers(1, &vbo_);
                if (vao_ != 0)
                    glDeleteVertexArrays(1, &vao_);
                vbo_ = 0;
                vao_ = 0;
                destroy_shader(default_2d_shader_);
                default_2d_shader_ = 0;
                destroy_texture(white_texture_);
                white_texture_ = 0;
                active_2d_shader_ = 0;
                last_2d_width_ = -1;
                last_2d_height_ = -1;
                last_offset_x_ = -1.0f;
                last_offset_y_ = -1.0f;
                premultiplied_alpha_ = false;
            }

            bool begin_2d(int width, int height) override
            {
                if (!initialise_2d())
                    return false;
                const float off_x = detail::screen_offset_x();
                const float off_y = detail::screen_offset_y();
                if (active_2d_shader_ != default_2d_shader_ || last_2d_width_ != width || last_2d_height_ != height ||
                    last_offset_x_ != off_x || last_offset_y_ != off_y)
                {
                    flush_2d();
                    float projection[16] = {};
                    projection[0] = width > 0 ? 2.0f / width : 0.0f;
                    projection[5] = height > 0 ? -2.0f / height : 0.0f;
                    projection[10] = -1.0f;
                    projection[12] = -1.0f + (width > 0 ? 2.0f * off_x / width : 0.0f);
                    projection[13] = 1.0f - (height > 0 ? 2.0f * off_y / height : 0.0f);
                    projection[15] = 1.0f;
                    use_shader(default_2d_shader_);
                    set_shader_mat4(default_2d_shader_, "uProjection", projection);
                    set_shader_int(default_2d_shader_, "uTexture", 0);
                    glEnable(GL_BLEND);
                    apply_blend_func();
                    glActiveTexture(GL_TEXTURE0);
                    active_2d_shader_ = default_2d_shader_;
                    last_2d_width_ = width;
                    last_2d_height_ = height;
                    last_offset_x_ = off_x;
                    last_offset_y_ = off_y;
                }
                return true;
            }
            bool begin_shader_2d(std::uint32_t program, int width, int height) override
            {
                if (!initialise_2d() || width <= 0 || height <= 0)
                    return false;
                flush_2d();
                if (!use_shader(program))
                    return false;
                float projection[16] = {};
                projection[0] = 2.0f / width;
                projection[5] = -2.0f / height;
                projection[10] = -1.0f;
                projection[12] = -1.0f + 2.0f * detail::screen_offset_x() / width;
                projection[13] = 1.0f - 2.0f * detail::screen_offset_y() / height;
                projection[15] = 1.0f;
                if (!set_shader_mat4(program, "uProjection", projection))
                    return false;
                // Custom shaders may name their main sampler differently, so a missing uTexture is not an error.
                set_shader_int(program, "uTexture", 0);
                glEnable(GL_BLEND);
                apply_blend_func();
                glActiveTexture(GL_TEXTURE0);
                active_2d_shader_ = program;
                last_2d_width_ = width;
                last_2d_height_ = height;
                last_offset_x_ = detail::screen_offset_x();
                last_offset_y_ = detail::screen_offset_y();
                return true;
            }
            void set_premultiplied_alpha(bool enabled) override
            {
                if (premultiplied_alpha_ != enabled)
                {
                    flush_2d();
                    premultiplied_alpha_ = enabled;
                    apply_blend_func();
                }
            }
            void set_blend_mode(BlendMode mode) override
            {
                if (blend_mode_ != mode)
                {
                    flush_2d();
                    blend_mode_ = mode;
                    apply_blend_func();
                }
            }
            bool clear_frame(float red, float green, float blue, float alpha) override
            {
                flush_2d();
                GLfloat previous_clear_color[4] = {};
                GLboolean previous_color_mask[4] = {};
                const GLboolean scissor_was_enabled = glIsEnabled(GL_SCISSOR_TEST);
                glGetFloatv(GL_COLOR_CLEAR_VALUE, previous_clear_color);
                glGetBooleanv(GL_COLOR_WRITEMASK, previous_color_mask);
                glDisable(GL_SCISSOR_TEST);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                context_.clear(red, green, blue, alpha);
                glClearColor(previous_clear_color[0], previous_clear_color[1], previous_clear_color[2], previous_clear_color[3]);
                glColorMask(previous_color_mask[0], previous_color_mask[1], previous_color_mask[2], previous_color_mask[3]);
                if (scissor_was_enabled)
                    glEnable(GL_SCISSOR_TEST);
                return true;
            }

            void set_clip_rect_2d(const ClipRect2D &clip) override
            {
                flush_2d();
                clip_ = clip;
                if (!clip.enabled)
                    glDisable(GL_SCISSOR_TEST);
            }

            void apply_clip_2d()
            {
                if (!clip_.enabled)
                    return;
                GLint viewport[4] = {};
                glGetIntegerv(GL_VIEWPORT, viewport);
                int x = 0, y = 0, w = 0, h = 0;
                clip_rect_to_pixels(clip_, last_2d_width_, last_2d_height_, viewport[2], viewport[3],
                                    detail::screen_offset_x(), detail::screen_offset_y(), x, y, w, h);
                glEnable(GL_SCISSOR_TEST);
                glScissor(viewport[0] + x, viewport[1] + viewport[3] - (y + h), w, h);
            }

            void flush_2d() override
            {
                if (batch_vertices_.empty() || !initialise_2d())
                    return;
                apply_clip_2d();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(batch_texture_ != 0 ? batch_texture_ : white_texture_));
                glBindVertexArray(vao_);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * batch_vertices_.size(), batch_vertices_.data(), GL_DYNAMIC_DRAW);
                GLenum mode = GL_TRIANGLES;
                switch (batch_primitive_)
                {
                case PrimitiveType::points:
                    mode = GL_POINTS;
                    break;
                case PrimitiveType::lines:
                    mode = GL_LINES;
                    break;
                case PrimitiveType::line_loop:
                    mode = GL_LINE_LOOP;
                    break;
                case PrimitiveType::triangles:
                    mode = GL_TRIANGLES;
                    break;
                case PrimitiveType::triangle_fan:
                    mode = GL_TRIANGLE_FAN;
                    break;
                }
                glDrawArrays(mode, 0, static_cast<GLsizei>(batch_vertices_.size()));
                glBindVertexArray(0);
                batch_vertices_.clear();
            }

            void submit_2d(PrimitiveType primitive_mode, const Vertex2D *vertices, int count, std::uint32_t texture) override
            {
                if (!vertices || count <= 0 || !initialise_2d())
                    return;
                const std::uint32_t resolved_texture = (texture != 0 ? texture : white_texture_);
                const PrimitiveType target_primitive = get_target_batch_primitive(primitive_mode);
                const int new_vertex_count = get_decomposed_vertex_count(primitive_mode, count);
                if (new_vertex_count <= 0)
                    return;

                if (!batch_vertices_.empty())
                {
                    if (batch_texture_ != resolved_texture ||
                        batch_primitive_ != target_primitive ||
                        batch_vertices_.size() + static_cast<std::size_t>(new_vertex_count) > MAX_BATCH_VERTICES)
                    {
                        flush_2d();
                    }
                }

                if (batch_vertices_.empty())
                {
                    batch_texture_ = resolved_texture;
                    batch_primitive_ = target_primitive;
                }

                PrimitiveType actual_primitive = batch_primitive_;
                append_decomposed_vertices(primitive_mode, vertices, count, actual_primitive, batch_vertices_);
            }

            bool create_storage_buffer(std::size_t size, std::uint32_t &buffer) override
            {
                GLuint handle = 0;
                glGenBuffers(1, &handle);
                buffer = handle;
                if (handle != 0)
                {
                    glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
                    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
                }
                return handle != 0;
            }
            void destroy_storage_buffer(std::uint32_t buffer) override
            {
                if (buffer != 0)
                {
                    const GLuint handle = static_cast<GLuint>(buffer);
                    glDeleteBuffers(1, &handle);
                }
            }
            bool upload_storage_buffer(std::uint32_t buffer, std::size_t size, const void *data, bool preserve_storage) override
            {
                if (buffer == 0)
                    return false;
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(buffer));
                if (!preserve_storage || data)
                    glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_DRAW);
                return true;
            }
            bool readback_storage_buffer(std::uint32_t buffer, std::size_t offset, std::size_t size, void *out_data) override
            {
                if (buffer == 0 || !out_data)
                    return false;
                flush_2d();
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(buffer));
                glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), out_data);
                return true;
            }
            void bind_storage_buffer(unsigned int binding, std::uint32_t buffer) override
            {
                flush_2d();
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, static_cast<GLuint>(buffer));
            }
            void bind_texture_unit(unsigned int unit, std::uint32_t texture) override
            {
                flush_2d();
                glActiveTexture(GL_TEXTURE0 + unit);
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texture));
            }
            void bind_storage_texture(unsigned int binding, std::uint32_t texture) override
            {
                flush_2d();
                if (glBindImageTexture)
                {
                    glBindImageTexture(binding, static_cast<GLuint>(texture), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
                }
            }
            void storage_barrier() override
            {
                flush_2d();
                glMemoryBarrier(GL_ALL_BARRIER_BITS);
            }

            bool create_shader(const ShaderSource &vertex_source, const ShaderSource &fragment_source,
                               std::uint32_t &program, std::string &error) override
            {
                program = 0;
                if (vertex_source.language != ShaderLanguage::glsl || fragment_source.language != ShaderLanguage::glsl)
                {
                    error = "OpenGL backend accepts GLSL shader sources only.";
                    return false;
                }
                const GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_source.text, error);
                if (vertex == 0)
                    return false;
                const GLuint fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_source.text, error);
                if (fragment == 0)
                {
                    glDeleteShader(vertex);
                    return false;
                }
                const GLuint linked = link_program(vertex, fragment, error);
                glDeleteShader(vertex);
                glDeleteShader(fragment);
                program = linked;
                return program != 0;
            }

            bool create_compute_shader(const ShaderSource &source, std::uint32_t &program,
                                       std::string &error) override
            {
                program = 0;
                if (source.language != ShaderLanguage::glsl)
                {
                    error = "OpenGL backend accepts GLSL shader sources only.";
                    return false;
                }
                const GLuint compute = compile_shader(GL_COMPUTE_SHADER, source.text, error);
                if (compute == 0)
                    return false;
                const GLuint linked = link_program(compute, 0, error);
                glDeleteShader(compute);
                program = linked;
                return program != 0;
            }
            bool create_shader_module(ShaderStage, const ShaderSource &, std::uint32_t &, std::string &error) override
            {
                error = "OpenGL renderer uses linked GLSL programs rather than standalone SPIR-V modules.";
                return false;
            }

            void destroy_shader(std::uint32_t program) override
            {
                if (program != 0)
                {
                    if (active_2d_shader_ == program)
                    {
                        flush_2d();
                        active_2d_shader_ = 0;
                    }
                    glDeleteProgram(static_cast<GLuint>(program));
                }
            }

            bool use_shader(std::uint32_t program) override
            {
                if (program == 0)
                    return false;
                if (active_2d_shader_ != program)
                {
                    flush_2d();
                    active_2d_shader_ = program;
                }
                glUseProgram(static_cast<GLuint>(program));
                return true;
            }

            void stop_shader() override
            {
                flush_2d();
                active_2d_shader_ = 0;
                glUseProgram(0);
            }

            bool dispatch_compute(std::uint32_t program, unsigned int groups_x,
                                  unsigned int groups_y, unsigned int groups_z) override
            {
                flush_2d();
                if (!use_shader(program))
                    return false;
                glDispatchCompute(groups_x, groups_y, groups_z);
                return true;
            }

            bool set_shader_int(std::uint32_t program, const char *name, int value) override
            {
                return set_location(program, name, [value](GLint location)
                                    { glUniform1i(location, value); });
            }

            bool set_shader_float(std::uint32_t program, const char *name, float value) override
            {
                return set_location(program, name, [value](GLint location)
                                    { glUniform1f(location, value); });
            }

            bool set_shader_float2(std::uint32_t program, const char *name, float x, float y) override
            {
                return set_location(program, name, [x, y](GLint location)
                                    { glUniform2f(location, x, y); });
            }

            bool set_shader_int2(std::uint32_t program, const char *name, int x, int y) override
            {
                return set_location(program, name, [x, y](GLint location)
                                    { glUniform2i(location, x, y); });
            }

            bool set_shader_float3(std::uint32_t program, const char *name, float x, float y, float z) override
            {
                return set_location(program, name, [x, y, z](GLint location)
                                    { glUniform3f(location, x, y, z); });
            }

            bool set_shader_mat4(std::uint32_t program, const char *name, const float *matrix) override
            {
                return matrix && set_location(program, name,
                                              [matrix](GLint location)
                                              { glUniformMatrix4fv(location, 1, GL_FALSE, matrix); });
            }

        private:
            struct RenderTargetState
            {
                GLint draw_framebuffer = 0;
                GLint read_framebuffer = 0;
                GLint viewport[4] = {};
                GLuint resolve_framebuffer = 0;
                int width = 0;
                int height = 0;
            };
            struct MultisampleTarget
            {
                GLuint framebuffer = 0;
                GLuint color = 0;
                GLuint depth = 0;
                int samples = 1;
            };

            template <typename Setter>
            bool set_location(std::uint32_t program, const char *name, Setter setter)
            {
                if (!use_shader(program))
                    return false;
                const GLint location = glGetUniformLocation(static_cast<GLuint>(program), name);
                if (location < 0)
                    return false;
                setter(location);
                return true;
            }

            static constexpr std::size_t MAX_BATCH_VERTICES = 65536;

            GLContext context_;
            GLuint vao_ = 0;
            GLuint vbo_ = 0;
            std::uint32_t default_2d_shader_ = 0;
            std::uint32_t white_texture_ = 0;
            std::string shader_error_;
            std::unordered_map<GLuint, std::pair<int, int>> texture_sizes_;
            std::unordered_set<GLuint> mipmapped_textures_;
            std::vector<RenderTargetState> render_target_stack_;
            struct GLMesh
            {
                GLuint vao = 0;
                GLuint vbo = 0;
                GLuint ebo = 0;
                GLsizei index_count = 0;
                std::size_t vertex_count = 0;
            };
            std::unordered_map<GLuint, GLuint> depth_buffers_;
            std::unordered_map<GLuint, MultisampleTarget> multisample_targets_;
            std::unordered_map<std::uint32_t, GLMesh> meshes_;
            std::uint32_t next_mesh_ = 1;
            std::uint32_t mesh_program_ = 0;
            std::uint32_t mesh_instanced_program_ = 0;
            std::unordered_map<std::uint32_t, GLInstanceBuffer> instance_buffers_;
            std::uint32_t next_instance_buffer_ = 1;
            std::uint32_t terrain_program_ = 0;
            std::uint32_t sky_program_ = 0;
            std::uint32_t sky_vao_ = 0;

            std::vector<Vertex2D> batch_vertices_;
            PrimitiveType batch_primitive_ = PrimitiveType::triangles;
            std::uint32_t batch_texture_ = 0;
            bool premultiplied_alpha_ = false;
            BlendMode blend_mode_ = BlendMode::normal;
            /** Apply the GL blend function matching the current premultiplied-alpha/blend-mode state. */
            void apply_blend_func()
            {
                if (blend_mode_ == BlendMode::additive)
                    glBlendFunc(GL_ONE, GL_ONE);
                else if (blend_mode_ == BlendMode::multiply)
                    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                else if (blend_mode_ == BlendMode::screen)
                    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
                else if (blend_mode_ == BlendMode::premultiplied)
                    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                else
                    glBlendFunc(premultiplied_alpha_ ? GL_ONE : GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            std::uint32_t active_2d_shader_ = 0;
            ClipRect2D clip_;
            int last_2d_width_ = -1;
            int last_2d_height_ = -1;
            float last_offset_x_ = -1.0f;
            float last_offset_y_ = -1.0f;
        };
    }

    std::unique_ptr<Renderer> create_gl_renderer()
    {
        return std::make_unique<GLRenderer>();
    }
}
