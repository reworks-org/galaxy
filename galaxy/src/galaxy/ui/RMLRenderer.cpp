///
/// RMLRenderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <RmlUi/Core/Core.h>
#include <stb_image.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "RMLRenderer.hpp"

#define RMLUI_SHADER_HEADER "#version 330\n"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26401)
#pragma warning(disable : 26409)
#pragma warning(disable : 26827)
#endif

namespace galaxy
{
	namespace ui
	{
		// clang-format off
		const char* shader_main_vertex = RMLUI_SHADER_HEADER R"(
			uniform vec2 _translate;
			uniform mat4 _transform;

			in vec2 inPosition;
			in vec4 inColor0;
			in vec2 inTexCoord0;

			out vec2 fragTexCoord;
			out vec4 fragColor;

			void main() {
				fragTexCoord = inTexCoord0;
				fragColor = inColor0;

				vec2 translatedPos = inPosition + _translate.xy;
				vec4 outPos = _transform * vec4(translatedPos, 0, 1);

				gl_Position = outPos;
			}
		)";

		const char* shader_main_fragment_texture = RMLUI_SHADER_HEADER R"(
			uniform sampler2D _tex;
			in vec2 fragTexCoord;
			in vec4 fragColor;

			out vec4 finalColor;

			void main() {
				vec4 texColor = texture(_tex, fragTexCoord);
				finalColor = fragColor * texColor;
			}
		)";

		const char* shader_main_fragment_color = RMLUI_SHADER_HEADER R"(
			in vec2 fragTexCoord;
			in vec4 fragColor;

			out vec4 finalColor;

			void main() {
				finalColor = fragColor;
			}
		)";
		// clang-format on

		const char* const program_uniform_names[(size_t)RMLProgramUniform::Count]   = {"_translate", "_transform", "_tex"};
		const char* const vertex_attribute_names[(size_t)RMLVertexAttribute::Count] = {"inPosition", "inColor0", "inTexCoord0"};

		GLuint create_shader(GLenum shader_type, const char* code_string)
		{
			const GLuint id = glCreateShader(shader_type);

			glShaderSource(id, 1, static_cast<const GLchar**>(&code_string), nullptr);
			glCompileShader(id);

			GLint status = 0;
			glGetShaderiv(id, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				GALAXY_LOG(GALAXY_ERROR, "Compile failure in RML OpenGL shader.");
				glDeleteShader(id);
				return 0;
			}

			return id;
		}

		void bind_attrib_locations(GLuint program)
		{
			for (GLuint i = 0; i < static_cast<GLuint>(RMLVertexAttribute::Count); i++)
			{
				glBindAttribLocation(program, i, vertex_attribute_names[i]);
			}
		}

		bool create_program(GLuint vertex_shader, GLuint fragment_shader, RMLProgramData& out_program)
		{
			const GLuint id = glCreateProgram();

			bind_attrib_locations(id);

			glAttachShader(id, vertex_shader);
			glAttachShader(id, fragment_shader);

			glLinkProgram(id);

			glDetachShader(id, vertex_shader);
			glDetachShader(id, fragment_shader);

			GLint status = 0;
			glGetProgramiv(id, GL_LINK_STATUS, &status);
			if (status == GL_FALSE)
			{
				GALAXY_LOG(GALAXY_ERROR, "RML OpenGL program linking failure.");
				glDeleteProgram(id);
				return false;
			}

			out_program    = {};
			out_program.id = id;

			// Make a lookup table for the uniform locations.
			GLint num_active_uniforms = 0;
			glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &num_active_uniforms);

			constexpr size_t name_size = 64;
			GLchar name_buf[name_size] = "";
			for (int unif = 0; unif < num_active_uniforms; ++unif)
			{
				GLint array_size      = 0;
				GLenum type           = 0;
				GLsizei actual_length = 0;
				glGetActiveUniform(id, unif, name_size, &actual_length, &array_size, &type, name_buf);
				const GLint location = glGetUniformLocation(id, name_buf);

				// See if we have the name in our pre-defined name list.
				RMLProgramUniform program_uniform = RMLProgramUniform::Count;
				for (int i = 0; i < static_cast<int>(RMLProgramUniform::Count); i++)
				{
					const char* uniform_name = program_uniform_names[i];
					if (strcmp(name_buf, uniform_name) == 0)
					{
						program_uniform = static_cast<RMLProgramUniform>(i);
						break;
					}
				}

				if (static_cast<size_t>(program_uniform) < static_cast<size_t>(RMLProgramUniform::Count))
				{
					out_program.uniform_locations[static_cast<size_t>(program_uniform)] = location;
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "OpenGL program uses unknown uniform '{0}'.", name_buf);
					return false;
				}
			}

			return true;
		}

		bool create_shaders(RMLShaderData& out_shaders)
		{
			out_shaders                   = {};
			GLuint& main_vertex           = out_shaders.shader_main_vertex;
			GLuint& main_fragment_color   = out_shaders.shader_main_fragment_color;
			GLuint& main_fragment_texture = out_shaders.shader_main_fragment_texture;

			main_vertex = create_shader(GL_VERTEX_SHADER, shader_main_vertex);
			if (!main_vertex)
			{
				GALAXY_LOG(GALAXY_ERROR, "Could not create OpenGL shader: 'shader_main_vertex'.");
				return false;
			}

			main_fragment_color = create_shader(GL_FRAGMENT_SHADER, shader_main_fragment_color);
			if (!main_fragment_color)
			{
				GALAXY_LOG(GALAXY_ERROR, "Could not create OpenGL shader: 'shader_main_fragment_color'.");
				return false;
			}

			main_fragment_texture = create_shader(GL_FRAGMENT_SHADER, shader_main_fragment_texture);
			if (!main_fragment_texture)
			{
				GALAXY_LOG(GALAXY_ERROR, "Could not create OpenGL shader: 'shader_main_fragment_texture'.");
				return false;
			}

			if (!create_program(main_vertex, main_fragment_color, out_shaders.program_color))
			{
				GALAXY_LOG(GALAXY_ERROR, "Could not create OpenGL program: 'program_color'.");
				return false;
			}

			if (!create_program(main_vertex, main_fragment_texture, out_shaders.program_texture))
			{
				GALAXY_LOG(GALAXY_ERROR, "Could not create OpenGL program: 'program_texture'.");
				return false;
			}

			return true;
		}

		void destroy_shaders(RMLShaderData& shaders)
		{
			glDeleteProgram(shaders.program_color.id);
			glDeleteProgram(shaders.program_texture.id);

			glDeleteShader(shaders.shader_main_vertex);
			glDeleteShader(shaders.shader_main_fragment_color);
			glDeleteShader(shaders.shader_main_fragment_texture);

			shaders = {};
		}

		RMLRenderer::RMLRenderer()
			: shaders {}
		{
		}

		RMLRenderer::~RMLRenderer()
		{
		}

		void RMLRenderer::init()
		{
			create_shaders(shaders);
		}

		void RMLRenderer::destroy()
		{
			destroy_shaders(shaders);
		}

		void RMLRenderer::begin_frame()
		{
			// Backup GL state.
			glstate_backup.enable_cull_face    = glIsEnabled(GL_CULL_FACE);
			glstate_backup.enable_blend        = glIsEnabled(GL_BLEND);
			glstate_backup.enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
			glstate_backup.enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

			glGetIntegerv(GL_VIEWPORT, glstate_backup.viewport);
			glGetIntegerv(GL_SCISSOR_BOX, glstate_backup.scissor);

			glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &glstate_backup.stencil_clear_value);
			glGetFloatv(GL_COLOR_CLEAR_VALUE, glstate_backup.color_clear_value);

			glGetIntegerv(GL_BLEND_EQUATION_RGB, &glstate_backup.blend_equation_rgb);
			glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &glstate_backup.blend_equation_alpha);
			glGetIntegerv(GL_BLEND_SRC_RGB, &glstate_backup.blend_src_rgb);
			glGetIntegerv(GL_BLEND_DST_RGB, &glstate_backup.blend_dst_rgb);
			glGetIntegerv(GL_BLEND_SRC_ALPHA, &glstate_backup.blend_src_alpha);
			glGetIntegerv(GL_BLEND_DST_ALPHA, &glstate_backup.blend_dst_alpha);

			glGetIntegerv(GL_STENCIL_FUNC, &glstate_backup.stencil_front.func);
			glGetIntegerv(GL_STENCIL_REF, &glstate_backup.stencil_front.ref);
			glGetIntegerv(GL_STENCIL_VALUE_MASK, &glstate_backup.stencil_front.value_mask);
			glGetIntegerv(GL_STENCIL_WRITEMASK, &glstate_backup.stencil_front.writemask);
			glGetIntegerv(GL_STENCIL_FAIL, &glstate_backup.stencil_front.fail);
			glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &glstate_backup.stencil_front.pass_depth_fail);
			glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &glstate_backup.stencil_front.pass_depth_pass);

			glGetIntegerv(GL_STENCIL_BACK_FUNC, &glstate_backup.stencil_back.func);
			glGetIntegerv(GL_STENCIL_BACK_REF, &glstate_backup.stencil_back.ref);
			glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &glstate_backup.stencil_back.value_mask);
			glGetIntegerv(GL_STENCIL_BACK_WRITEMASK, &glstate_backup.stencil_back.writemask);
			glGetIntegerv(GL_STENCIL_BACK_FAIL, &glstate_backup.stencil_back.fail);
			glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &glstate_backup.stencil_back.pass_depth_fail);
			glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &glstate_backup.stencil_back.pass_depth_pass);

			// Setup expected GL state.
			auto& window = core::ServiceLocator<core::Window>::ref();
			glViewport(0, 0, window.get_width(), window.get_height());

			glClearStencil(0);
			glClearColor(0, 0, 0, 1);

			glDisable(GL_CULL_FACE);

			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, -1);
			glStencilMask(-1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			projection = Rml::Matrix4f::ProjectOrtho(0.0f, window.get_widthf(), window.get_heightf(), 0.0f, -10000.0f, 10000.0f);
			SetTransform(nullptr);
		}

		void RMLRenderer::end_frame()
		{
			// Restore GL state.
			if (glstate_backup.enable_cull_face)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}

			if (glstate_backup.enable_blend)
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			if (glstate_backup.enable_stencil_test)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}

			if (glstate_backup.enable_scissor_test)
			{
				glEnable(GL_SCISSOR_TEST);
			}
			else
			{
				glDisable(GL_SCISSOR_TEST);
			}

			glViewport(glstate_backup.viewport[0], glstate_backup.viewport[1], glstate_backup.viewport[2], glstate_backup.viewport[3]);
			glScissor(glstate_backup.scissor[0], glstate_backup.scissor[1], glstate_backup.scissor[2], glstate_backup.scissor[3]);

			glClearStencil(glstate_backup.stencil_clear_value);
			glClearColor(glstate_backup.color_clear_value[0],
				glstate_backup.color_clear_value[1],
				glstate_backup.color_clear_value[2],
				glstate_backup.color_clear_value[3]);

			glBlendEquationSeparate(glstate_backup.blend_equation_rgb, glstate_backup.blend_equation_alpha);
			glBlendFuncSeparate(glstate_backup.blend_src_rgb, glstate_backup.blend_dst_rgb, glstate_backup.blend_src_alpha, glstate_backup.blend_dst_alpha);

			glStencilFuncSeparate(GL_FRONT, glstate_backup.stencil_front.func, glstate_backup.stencil_front.ref, glstate_backup.stencil_front.value_mask);
			glStencilMaskSeparate(GL_FRONT, glstate_backup.stencil_front.writemask);
			glStencilOpSeparate(GL_FRONT,
				glstate_backup.stencil_front.fail,
				glstate_backup.stencil_front.pass_depth_fail,
				glstate_backup.stencil_front.pass_depth_pass);

			glStencilFuncSeparate(GL_BACK, glstate_backup.stencil_back.func, glstate_backup.stencil_back.ref, glstate_backup.stencil_back.value_mask);
			glStencilMaskSeparate(GL_BACK, glstate_backup.stencil_back.writemask);
			glStencilOpSeparate(GL_BACK,
				glstate_backup.stencil_back.fail,
				glstate_backup.stencil_back.pass_depth_fail,
				glstate_backup.stencil_back.pass_depth_pass);
		}

		void RMLRenderer::clear()
		{
			glClearStencil(0);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void RMLRenderer::RenderGeometry(Rml::Vertex* vertices,
			int num_vertices,
			int* indices,
			int num_indices,
			const Rml::TextureHandle texture,
			const Rml::Vector2f& translation)
		{
			const auto geometry = CompileGeometry(vertices, num_vertices, indices, num_indices, texture);
			if (geometry)
			{
				RenderCompiledGeometry(geometry, translation);
				ReleaseCompiledGeometry(geometry);
			}
		}

		Rml::CompiledGeometryHandle
		RMLRenderer::CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture)
		{
			constexpr GLenum draw_usage = GL_STATIC_DRAW;

			GLuint vao = 0;
			GLuint vbo = 0;
			GLuint ibo = 0;

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ibo);
			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Rml::Vertex) * num_vertices, vertices, draw_usage);

			glEnableVertexAttribArray(static_cast<GLuint>(RMLVertexAttribute::Position));
			glVertexAttribPointer(static_cast<GLuint>(RMLVertexAttribute::Position),
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Rml::Vertex),
				(const GLvoid*)(offsetof(Rml::Vertex, position)));

			glEnableVertexAttribArray(static_cast<GLuint>(RMLVertexAttribute::Color0));
			glVertexAttribPointer(static_cast<GLuint>(RMLVertexAttribute::Color0),
				4,
				GL_UNSIGNED_BYTE,
				GL_TRUE,
				sizeof(Rml::Vertex),
				(const GLvoid*)(offsetof(Rml::Vertex, colour)));

			glEnableVertexAttribArray(static_cast<GLuint>(RMLVertexAttribute::TexCoord0));
			glVertexAttribPointer(static_cast<GLuint>(RMLVertexAttribute::TexCoord0),
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Rml::Vertex),
				(const GLvoid*)(offsetof(Rml::Vertex, tex_coord)));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, draw_usage);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			auto geometry        = new RMLCompiledGeometryData();
			geometry->texture    = texture;
			geometry->vao        = vao;
			geometry->vbo        = vbo;
			geometry->ibo        = ibo;
			geometry->draw_count = num_indices;

			return reinterpret_cast<Rml::CompiledGeometryHandle>(geometry);
		}

		void RMLRenderer::RenderCompiledGeometry(Rml::CompiledGeometryHandle handle, const Rml::Vector2f& translation)
		{
			const RMLCompiledGeometryData* geometry = reinterpret_cast<RMLCompiledGeometryData*>(handle);

			if (geometry->texture)
			{
				glUseProgram(shaders.program_texture.id);

				if (geometry->texture != -1)
				{
					glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(geometry->texture));
				}

				submit_transform_uniform(RMLProgramId::Texture, shaders.program_texture.uniform_locations[static_cast<size_t>(RMLProgramUniform::Transform)]);
				glUniform2fv(shaders.program_texture.uniform_locations[static_cast<size_t>(RMLProgramUniform::Translate)], 1, &translation.x);
			}
			else
			{
				glUseProgram(shaders.program_color.id);
				glBindTexture(GL_TEXTURE_2D, 0);
				submit_transform_uniform(RMLProgramId::Color, shaders.program_color.uniform_locations[static_cast<size_t>(RMLProgramUniform::Transform)]);
				glUniform2fv(shaders.program_color.uniform_locations[static_cast<size_t>(RMLProgramUniform::Translate)], 1, &translation.x);
			}

			glBindVertexArray(geometry->vao);
			glDrawElements(GL_TRIANGLES, geometry->draw_count, GL_UNSIGNED_INT, nullptr);

			glBindVertexArray(0);
			glUseProgram(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void RMLRenderer::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle handle)
		{
			auto geometry = reinterpret_cast<RMLCompiledGeometryData*>(handle);

			glDeleteVertexArrays(1, &geometry->vao);
			glDeleteBuffers(1, &geometry->vbo);
			glDeleteBuffers(1, &geometry->ibo);

			delete geometry;
		}

		void RMLRenderer::EnableScissorRegion(bool enable)
		{
			auto new_state = RMLScissoringState::Disable;

			if (enable)
			{
				new_state = (transform_active ? RMLScissoringState::RMLStencil : RMLScissoringState::Scissor);
			}

			if (new_state != scissoring_state)
			{
				if (scissoring_state == RMLScissoringState::Scissor)
				{
					glDisable(GL_SCISSOR_TEST);
				}
				else if (scissoring_state == RMLScissoringState::RMLStencil)
				{
					glStencilFunc(GL_ALWAYS, 1, -1);
				}

				if (new_state == RMLScissoringState::Scissor)
				{
					glEnable(GL_SCISSOR_TEST);
				}
				else if (new_state == RMLScissoringState::RMLStencil)
				{
					glStencilFunc(GL_EQUAL, 1, -1);
				}

				scissoring_state = new_state;
			}
		}

		void RMLRenderer::SetScissorRegion(int x, int y, int width, int height)
		{
			if (transform_active)
			{
				const auto left   = static_cast<float>(x);
				const auto right  = static_cast<float>(x + width);
				const auto top    = static_cast<float>(y);
				const auto bottom = static_cast<float>(y + height);

				Rml::Vertex vertices[4];
				vertices[0].position = {left, top};
				vertices[1].position = {right, top};
				vertices[2].position = {right, bottom};
				vertices[3].position = {left, bottom};

				int indices[6] = {0, 2, 1, 0, 3, 2};

				glClear(GL_STENCIL_BUFFER_BIT);
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
				glStencilFunc(GL_ALWAYS, 1, -1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

				RenderGeometry(vertices, 4, indices, 6, 0, {0, 0});

				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				glStencilFunc(GL_EQUAL, 1, -1);
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			}
			else
			{
				auto& window = core::ServiceLocator<core::Window>::ref();
				glScissor(x, window.get_height() - (y + height), width, height);
			}
		}

		bool RMLRenderer::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
		{
			bool result = true;
			auto& fs    = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto file_info = fs.find(source);
			if (file_info.code == fs::FileCode::FOUND)
			{
				stbi_set_flip_vertically_on_load(false);
				unsigned char* data = stbi_load(file_info.string.c_str(), &texture_dimensions.x, &texture_dimensions.y, nullptr, STBI_rgb_alpha);

				if (data)
				{
					result = GenerateTexture(texture_handle, data, texture_dimensions);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load RML texture: {0}. {1}.", source, stbi_failure_reason());
					result = false;
				}

				stbi_image_free(data);
				stbi_set_flip_vertically_on_load(true);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find RML texture '{0}', because {1}.", source, magic_enum::enum_name(file_info.code));
				result = false;
			}

			return result;
		}

		bool RMLRenderer::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
		{
			GLuint texture_id = 0;
			glGenTextures(1, &texture_id);
			if (texture_id == 0)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to generate texture.");
				return false;
			}

			glBindTexture(GL_TEXTURE_2D, texture_id);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			texture_handle = static_cast<Rml::TextureHandle>(texture_id);

			glBindTexture(GL_TEXTURE_2D, 0);

			return true;
		}

		void RMLRenderer::ReleaseTexture(Rml::TextureHandle texture_handle)
		{
			glDeleteTextures(1, reinterpret_cast<GLuint*>(&texture_handle));
		}

		void RMLRenderer::SetTransform(const Rml::Matrix4f* new_transform)
		{
			transform_active      = (new_transform != nullptr);
			transform             = projection * (new_transform ? *new_transform : Rml::Matrix4f::Identity());
			transform_dirty_state = RMLProgramId::All;
		}

		void RMLRenderer::submit_transform_uniform(RMLProgramId program_id, int uniform_location)
		{
			if (static_cast<int>(program_id) & static_cast<int>(transform_dirty_state))
			{
				glUniformMatrix4fv(uniform_location, 1, false, transform.data());
				transform_dirty_state = static_cast<RMLProgramId>(static_cast<int>(transform_dirty_state) & ~static_cast<int>(program_id));
			}
		}
	} // namespace ui
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif