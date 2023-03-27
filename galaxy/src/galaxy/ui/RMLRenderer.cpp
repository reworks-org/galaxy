///
/// RMLRenderer.cpp
/// galaxy
///
/// Originally from https://github.com/mikke89/RmlUi/blob/master/Backends/RmlUi_Renderer_GL3.cpp
/// Code has been modified to work in galaxy.
///

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Platform.h>
#include <stb_image.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "RMLRenderer.hpp"
#if defined(RMLUI_PLATFORM_WIN32) && !defined(__MINGW32__)
// function call missing argument list
#pragma warning(disable : 4551)
// unreferenced local function has been removed
#pragma warning(disable : 4505)
#endif

#define RMLUI_SHADER_HEADER "#version 330\n"

static const char* shader_main_vertex = RMLUI_SHADER_HEADER R"(
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

static const char* shader_main_fragment_texture = RMLUI_SHADER_HEADER R"(
uniform sampler2D _tex;
in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

void main() {
	vec4 texColor = texture(_tex, fragTexCoord);
	finalColor = fragColor * texColor;
}
)";
static const char* shader_main_fragment_color   = RMLUI_SHADER_HEADER R"(
in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

void main() {
	finalColor = fragColor;
}
)";

namespace Gfx
{

	enum class ProgramUniform
	{
		Translate,
		Transform,
		Tex,
		Count
	};
	static const char* const program_uniform_names[(size_t)ProgramUniform::Count] = {"_translate", "_transform", "_tex"};

	enum class VertexAttribute
	{
		Position,
		Color0,
		TexCoord0,
		Count
	};
	static const char* const vertex_attribute_names[(size_t)VertexAttribute::Count] = {"inPosition", "inColor0", "inTexCoord0"};

	struct CompiledGeometryData
	{
		Rml::TextureHandle texture;
		GLuint vao;
		GLuint vbo;
		GLuint ibo;
		GLsizei draw_count;
	};

	struct ProgramData
	{
		GLuint id;
		GLint uniform_locations[(size_t)ProgramUniform::Count];
	};

	struct ShadersData
	{
		ProgramData program_color;
		ProgramData program_texture;
		GLuint shader_main_vertex;
		GLuint shader_main_fragment_color;
		GLuint shader_main_fragment_texture;
	};

	// Create the shader, 'shader_type' is either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
	static GLuint CreateShader(GLenum shader_type, const char* code_string)
	{
		GLuint id = glCreateShader(shader_type);

		glShaderSource(id, 1, (const GLchar**)&code_string, NULL);
		glCompileShader(id);

		GLint status = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint info_log_length = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
			char* info_log_string = new char[info_log_length + 1];
			glGetShaderInfoLog(id, info_log_length, NULL, info_log_string);

			Rml::Log::Message(Rml::Log::LT_ERROR, "Compile failure in OpenGL shader: %s", info_log_string);
			delete[] info_log_string;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	static void BindAttribLocations(GLuint program)
	{
		for (GLuint i = 0; i < (GLuint)VertexAttribute::Count; i++)
		{
			glBindAttribLocation(program, i, vertex_attribute_names[i]);
		}
	}

	static bool CreateProgram(GLuint vertex_shader, GLuint fragment_shader, ProgramData& out_program)
	{
		GLuint id = glCreateProgram();
		RMLUI_ASSERT(id);

		BindAttribLocations(id);

		glAttachShader(id, vertex_shader);
		glAttachShader(id, fragment_shader);

		glLinkProgram(id);

		glDetachShader(id, vertex_shader);
		glDetachShader(id, fragment_shader);

		GLint status = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint info_log_length = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
			char* info_log_string = new char[info_log_length + 1];
			glGetProgramInfoLog(id, info_log_length, NULL, info_log_string);

			Rml::Log::Message(Rml::Log::LT_ERROR, "OpenGL program linking failure: %s", info_log_string);
			delete[] info_log_string;
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
			GLint location = glGetUniformLocation(id, name_buf);

			// See if we have the name in our pre-defined name list.
			ProgramUniform program_uniform = ProgramUniform::Count;
			for (int i = 0; i < (int)ProgramUniform::Count; i++)
			{
				const char* uniform_name = program_uniform_names[i];
				if (strcmp(name_buf, uniform_name) == 0)
				{
					program_uniform = (ProgramUniform)i;
					break;
				}
			}

			if ((size_t)program_uniform < (size_t)ProgramUniform::Count)
			{
				out_program.uniform_locations[(size_t)program_uniform] = location;
			}
			else
			{
				Rml::Log::Message(Rml::Log::LT_ERROR, "OpenGL program uses unknown uniform '%s'.", name_buf);
				return false;
			}
		}

		return true;
	}

	static bool CreateShaders(ShadersData& out_shaders)
	{
		out_shaders                   = {};
		GLuint& main_vertex           = out_shaders.shader_main_vertex;
		GLuint& main_fragment_color   = out_shaders.shader_main_fragment_color;
		GLuint& main_fragment_texture = out_shaders.shader_main_fragment_texture;

		main_vertex = CreateShader(GL_VERTEX_SHADER, shader_main_vertex);
		if (!main_vertex)
		{
			Rml::Log::Message(Rml::Log::LT_ERROR, "Could not create OpenGL shader: 'shader_main_vertex'.");
			return false;
		}
		main_fragment_color = CreateShader(GL_FRAGMENT_SHADER, shader_main_fragment_color);
		if (!main_fragment_color)
		{
			Rml::Log::Message(Rml::Log::LT_ERROR, "Could not create OpenGL shader: 'shader_main_fragment_color'.");
			return false;
		}
		main_fragment_texture = CreateShader(GL_FRAGMENT_SHADER, shader_main_fragment_texture);
		if (!main_fragment_texture)
		{
			Rml::Log::Message(Rml::Log::LT_ERROR, "Could not create OpenGL shader: 'shader_main_fragment_texture'.");
			return false;
		}

		if (!CreateProgram(main_vertex, main_fragment_color, out_shaders.program_color))
		{
			Rml::Log::Message(Rml::Log::LT_ERROR, "Could not create OpenGL program: 'program_color'.");
			return false;
		}
		if (!CreateProgram(main_vertex, main_fragment_texture, out_shaders.program_texture))
		{
			Rml::Log::Message(Rml::Log::LT_ERROR, "Could not create OpenGL program: 'program_texture'.");
			return false;
		}

		return true;
	}

	static void DestroyShaders(ShadersData& shaders)
	{
		glDeleteProgram(shaders.program_color.id);
		glDeleteProgram(shaders.program_texture.id);

		glDeleteShader(shaders.shader_main_vertex);
		glDeleteShader(shaders.shader_main_fragment_color);
		glDeleteShader(shaders.shader_main_fragment_texture);

		shaders = {};
	}

} // namespace Gfx

namespace galaxy
{
	namespace ui
	{
		RMLRenderer::RMLRenderer()
			: transform_dirty_state {ProgramId::All}
			, transform_active {false}
			, scissoring_state {ScissoringState::Disable}
			, shaders {nullptr}
			, m_window {nullptr}
		{
		}

		RMLRenderer::~RMLRenderer()
		{
		}

		void RMLRenderer::init()
		{
			m_window = &core::ServiceLocator<core::Window>::ref();

			shaders = Rml::MakeUnique<Gfx::ShadersData>();

			if (!Gfx::CreateShaders(*shaders))
				shaders.reset();
		}

		void RMLRenderer::destroy()
		{
			if (shaders)
				Gfx::DestroyShaders(*shaders);
		}

		void RMLRenderer::begin_frame()
		{
			glGetIntegerv(GL_VIEWPORT, m_viewport_backup);

			const auto viewport = m_window->get_framebuffer_size();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, viewport.x, viewport.y);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, GLuint(-1));
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			projection = Rml::Matrix4f::ProjectOrtho(0, (float)viewport.x, (float)viewport.y, 0, -10000, 10000);

			SetTransform(nullptr);

			glClearStencil(0);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void RMLRenderer::end_frame()
		{
			glfwSwapBuffers(core::ServiceLocator<core::Window>::ref().handle());

			glDisable(GL_STENCIL_TEST);
			glDisable(GL_SCISSOR_TEST);
			glViewport(m_viewport_backup[0], m_viewport_backup[1], m_viewport_backup[2], m_viewport_backup[3]);

			scissoring_state = ScissoringState::Disable;
		}

		void RMLRenderer::RenderGeometry(Rml::Vertex* vertices,
			int num_vertices,
			int* indices,
			int num_indices,
			Rml::TextureHandle texture,
			const Rml::Vector2f& translation)
		{
			Rml::CompiledGeometryHandle geometry = CompileGeometry(vertices, num_vertices, indices, num_indices, texture);

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
			glBufferData(GL_ARRAY_BUFFER, sizeof(Rml::Vertex) * num_vertices, (const void*)vertices, draw_usage);

			glEnableVertexAttribArray((GLuint)Gfx::VertexAttribute::Position);
			glVertexAttribPointer((GLuint)Gfx::VertexAttribute::Position,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Rml::Vertex),
				(const GLvoid*)(offsetof(Rml::Vertex, position)));

			glEnableVertexAttribArray((GLuint)Gfx::VertexAttribute::Color0);
			glVertexAttribPointer((GLuint)Gfx::VertexAttribute::Color0,
				4,
				GL_UNSIGNED_BYTE,
				GL_TRUE,
				sizeof(Rml::Vertex),
				(const GLvoid*)(offsetof(Rml::Vertex, colour)));

			glEnableVertexAttribArray((GLuint)Gfx::VertexAttribute::TexCoord0);
			glVertexAttribPointer((GLuint)Gfx::VertexAttribute::TexCoord0,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Rml::Vertex),
				(const GLvoid*)(offsetof(Rml::Vertex, tex_coord)));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, (const void*)indices, draw_usage);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			Gfx::CompiledGeometryData* geometry = new Gfx::CompiledGeometryData;
			geometry->texture                   = texture;
			geometry->vao                       = vao;
			geometry->vbo                       = vbo;
			geometry->ibo                       = ibo;
			geometry->draw_count                = num_indices;

			return (Rml::CompiledGeometryHandle)geometry;
		}

		void RMLRenderer::RenderCompiledGeometry(Rml::CompiledGeometryHandle handle, const Rml::Vector2f& translation)
		{
			Gfx::CompiledGeometryData* geometry = (Gfx::CompiledGeometryData*)handle;

			if (geometry->texture)
			{
				glUseProgram(shaders->program_texture.id);
				if (geometry->texture != TextureEnableWithoutBinding)
					glBindTexture(GL_TEXTURE_2D, (GLuint)geometry->texture);
				SubmitTransformUniform(ProgramId::Texture, shaders->program_texture.uniform_locations[(size_t)Gfx::ProgramUniform::Transform]);
				glUniform2fv(shaders->program_texture.uniform_locations[(size_t)Gfx::ProgramUniform::Translate], 1, &translation.x);
			}
			else
			{
				glUseProgram(shaders->program_color.id);
				glBindTexture(GL_TEXTURE_2D, 0);
				SubmitTransformUniform(ProgramId::Color, shaders->program_color.uniform_locations[(size_t)Gfx::ProgramUniform::Transform]);
				glUniform2fv(shaders->program_color.uniform_locations[(size_t)Gfx::ProgramUniform::Translate], 1, &translation.x);
			}

			glBindVertexArray(geometry->vao);
			glDrawElements(GL_TRIANGLES, geometry->draw_count, GL_UNSIGNED_INT, (const GLvoid*)0);

			glUseProgram(0);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void RMLRenderer::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle handle)
		{
			Gfx::CompiledGeometryData* geometry = (Gfx::CompiledGeometryData*)handle;

			glDeleteVertexArrays(1, &geometry->vao);
			glDeleteBuffers(1, &geometry->vbo);
			glDeleteBuffers(1, &geometry->ibo);

			delete geometry;
		}

		void RMLRenderer::EnableScissorRegion(bool enable)
		{
			ScissoringState new_state = ScissoringState::Disable;

			if (enable)
				new_state = (transform_active ? ScissoringState::Stencil : ScissoringState::Scissor);

			if (new_state != scissoring_state)
			{
				// Disable old
				if (scissoring_state == ScissoringState::Scissor)
					glDisable(GL_SCISSOR_TEST);
				else if (scissoring_state == ScissoringState::Stencil)
					glStencilFunc(GL_ALWAYS, 1, GLuint(-1));

				// Enable new
				if (new_state == ScissoringState::Scissor)
					glEnable(GL_SCISSOR_TEST);
				else if (new_state == ScissoringState::Stencil)
					glStencilFunc(GL_EQUAL, 1, GLuint(-1));

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
				glStencilFunc(GL_ALWAYS, 1, GLuint(-1));
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

				RenderGeometry(vertices, 4, indices, 6, 0, Rml::Vector2f(0, 0));

				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				glStencilFunc(GL_EQUAL, 1, GLuint(-1));
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			}
			else
			{
				const auto viewport = m_window->get_framebuffer_size();
				glScissor(x, viewport.y - (y + height), width, height);
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
				Rml::Log::Message(Rml::Log::LT_ERROR, "Failed to generate texture.");
				return false;
			}

			glBindTexture(GL_TEXTURE_2D, texture_id);

			GLint internal_format = GL_RGBA8;
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			texture_handle = (Rml::TextureHandle)texture_id;

			glBindTexture(GL_TEXTURE_2D, 0);

			return true;
		}

		void RMLRenderer::ReleaseTexture(Rml::TextureHandle texture)
		{
			glDeleteTextures(1, (GLuint*)&texture);
		}

		void RMLRenderer::SetTransform(const Rml::Matrix4f* new_transform)
		{
			transform_active      = (new_transform != nullptr);
			transform             = projection * (new_transform ? *new_transform : Rml::Matrix4f::Identity());
			transform_dirty_state = ProgramId::All;
		}

		void RMLRenderer::SubmitTransformUniform(ProgramId program_id, int uniform_location)
		{
			if ((int)program_id & (int)transform_dirty_state)
			{
				glUniformMatrix4fv(uniform_location, 1, false, transform.data());
				transform_dirty_state = ProgramId((int)transform_dirty_state & ~(int)program_id);
			}
		}
	} // namespace ui
} // namespace galaxy