/** @file
 * @brief Implements the OpenGL 4.3 Renderer backend.
 */
#define GL_GLEXT_PROTOTYPES

#include "gl_renderer.h"
#include "gl_context.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include <cstdint>
#include <cstring>
#include <vector>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <unordered_map>

namespace sl::detail
{
    namespace
    {
        std::string load_glsl_shader(const char *name)
        {
            std::ifstream file(std::filesystem::path(SIMLIB_GLSL_SHADER_DIR) / name);
            if (!file)
            {
                file.clear();
                file.open(std::filesystem::path(SIMLIB_BUILD_GLSL_SHADER_DIR) / name);
            }
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
            void configure_window() override
            {
                GLContext::configure_window_attributes();
            }
            std::uint32_t window_flags() const override { return SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE; }

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
                depth_buffers_.clear();
                multisample_targets_.clear();
                texture_sizes_.clear();
                render_target_stack_.clear();
                context_.shutdown();
            }

            bool resize(int width, int height, std::string &error) override
            {
                return context_.resize(width, height, error);
            }

            bool set_vsync(bool enabled) override
            {
                return context_.set_vsync(enabled);
            }

            bool vsync_active() const override
            {
                return context_.vsync_active();
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
                if (description.width <= 0 || description.height <= 0)
                {
                    return false;
                }
                GLuint handle = 0;
                glGenTextures(1, &handle);
                if (handle == 0)
                {
                    return false;
                }
                glBindTexture(GL_TEXTURE_2D, handle);
                const GLint filter = description.filter == TextureFilter::linear ? GL_LINEAR : GL_NEAREST;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, description.width, description.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                if (glGetError() != GL_NO_ERROR)
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
                glBindTexture(GL_TEXTURE_2D, handle);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                                GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                return true;
            }

            bool download_texture(std::uint32_t texture, int width, int height,
                                  std::uint8_t *out_pixels) override
            {
                if (texture == 0 || !out_pixels || width <= 0 || height <= 0)
                {
                    return false;
                }
                flush_2d();
                const GLuint handle = static_cast<GLuint>(texture);
                glBindTexture(GL_TEXTURE_2D, handle);
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, out_pixels);
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
                }
            }

            bool create_render_target(int width, int height, std::uint32_t &texture,
                                      std::uint32_t &framebuffer) override
            {
                texture = 0;
                framebuffer = 0;
                GLint previous_framebuffer = 0;
                GLint previous_viewport[4] = {};
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_framebuffer);
                glGetIntegerv(GL_VIEWPORT, previous_viewport);
                const auto restore_state = [&]()
                {
                    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(previous_framebuffer));
                    glViewport(previous_viewport[0], previous_viewport[1],
                               previous_viewport[2], previous_viewport[3]);
                };
                if (width <= 0 || height <= 0)
                {
                    return false;
                }
                if (!create_texture({width, height, TextureFilter::linear}, texture))
                {
                    return false;
                }
                GLuint fbo = 0;
                glGenFramebuffers(1, &fbo);
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                       static_cast<GLuint>(texture), 0);
                const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
                glGenFramebuffers(1, &target.framebuffer);
                glGenRenderbuffers(1, &target.color);
                glGenRenderbuffers(1, &target.depth);
                glBindRenderbuffer(GL_RENDERBUFFER, target.color);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
                glBindRenderbuffer(GL_RENDERBUFFER, target.depth);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
                GLint previous_framebuffer = 0;
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_framebuffer);
                glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, target.color);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, target.depth);
                const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
                glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(previous_framebuffer));
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
                GLint previous_framebuffer = 0;
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_framebuffer);
                GLuint depth = 0;
                glGenRenderbuffers(1, &depth);
                glBindRenderbuffer(GL_RENDERBUFFER, depth);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
                glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(framebuffer));
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
                const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
                glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(previous_framebuffer));
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
                if (!vertices || !indices || vertex_count == 0 || index_count == 0)
                    return false;
                GLMesh mesh;
                mesh.index_count = static_cast<GLsizei>(index_count);
                glGenVertexArrays(1, &mesh.vao);
                glGenBuffers(1, &mesh.vbo);
                glGenBuffers(1, &mesh.ebo);
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
                glBindVertexArray(0);
                handle = next_mesh_++;
                meshes_[handle] = mesh;
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
                glDepthMask(GL_TRUE);
                glClearColor(red, green, blue, alpha);
                glClearDepth(1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                return true;
            }

            bool draw_mesh(std::uint32_t handle, const MeshDrawParams &params) override
            {
                const auto found = meshes_.find(handle);
                if (found == meshes_.end())
                    return false;
                if (mesh_program_ == 0)
                {
                    std::string error;
                    if (!create_shader(ShaderSource::glsl(load_glsl_shader("mesh3d.vert")),
                                       ShaderSource::glsl(load_glsl_shader("mesh3d.frag")),
                                       mesh_program_, error))
                        return false;
                }
                flush_2d();
                GLint previous_program = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
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
                glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(params.texture != 0 ? params.texture : white_texture_));
                glUseProgram(mesh_program_);
                glUniform1i(glGetUniformLocation(mesh_program_, "uTexture"), 0);
                glUniformMatrix4fv(glGetUniformLocation(mesh_program_, "uMvp"), 1, GL_FALSE, params.mvp);
                glUniform4fv(glGetUniformLocation(mesh_program_, "uNormalRows"), 3, &params.normal_rows[0][0]);
                glUniform4fv(glGetUniformLocation(mesh_program_, "uLight"), 1, params.light);
                glBindVertexArray(found->second.vao);
                glDrawElements(GL_TRIANGLES, found->second.index_count, GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
                glUseProgram(static_cast<GLuint>(previous_program));
                if (!cull_was_enabled)
                    glDisable(GL_CULL_FACE);
                if (!depth_was_enabled)
                    glDisable(GL_DEPTH_TEST);
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
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous.framebuffer);
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
                    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(previous.framebuffer));
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
                glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous_fbo);
                glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(framebuffer));
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, out_pixels);
                glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(previous_fbo));

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
                if (!set_shader_mat4(program, "uProjection", projection) ||
                    !set_shader_int(program, "uTexture", 0))
                    return false;
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
                context_.clear(red, green, blue, alpha);
                return true;
            }

            void flush_2d() override
            {
                if (batch_vertices_.empty() || !initialise_2d())
                    return;
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
                GLint framebuffer = 0;
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
            std::vector<RenderTargetState> render_target_stack_;
            struct GLMesh
            {
                GLuint vao = 0;
                GLuint vbo = 0;
                GLuint ebo = 0;
                GLsizei index_count = 0;
            };
            std::unordered_map<GLuint, GLuint> depth_buffers_;
            std::unordered_map<GLuint, MultisampleTarget> multisample_targets_;
            std::unordered_map<std::uint32_t, GLMesh> meshes_;
            std::uint32_t next_mesh_ = 1;
            std::uint32_t mesh_program_ = 0;

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
                else
                    glBlendFunc(premultiplied_alpha_ ? GL_ONE : GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            std::uint32_t active_2d_shader_ = 0;
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
