///
/// RMLRenderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb/stb_image.h>

#include "galaxy/core/GalaxyConfig.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/VertexArray.hpp"

#include "RMLRenderer.hpp"

///
/// RML document vertex shader.
///
constexpr const char* const rml_vert = R"(
	#version 450 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;

	uniform vec2 u_translation;
	uniform mat4 u_transform;
	uniform mat4 u_proj;

	out vec2 io_texels;
	out vec4 io_colour;

	void main()
	{
		io_texels = l_texels;
		io_colour = l_colour;

		gl_Position  = u_proj * u_transform * vec4(l_pos + u_translation, 0.0, 1.0);
	}
)";

///
/// RML document fragment shader.
///
constexpr const char* const rml_frag = R"(
	#version 450 core

	out vec4 io_frag_colour;

	in vec2 io_texels;
	in vec4 io_colour;	

	uniform sampler2D u_texture;

	void main()
	{
		io_frag_colour = texture(u_texture, io_texels) * io_colour;
	}
)";

namespace galaxy
{
	struct RMLVAO final
	{
		graphics::VertexArray m_vao;
		GLuint                m_texture;
	};

	namespace ui
	{
		RMLRenderer::RMLRenderer() noexcept
			: m_identity {1.0f}
			, m_scissor_region {0.0f, 0.0f, 0.0f, 0.0f}
		{
			m_shader.load_raw(rml_vert, rml_frag);
			m_shader.bind();

			m_identity = glm::make_mat4(Rml::Matrix4f::Identity().data());
			m_shader.set_uniform("u_transform", m_identity);

			m_shader.unbind();
		}

		void RMLRenderer::RenderGeometry(Rml::Vertex* vertices,
			int                                       num_vertices,
			int*                                      indices,
			int                                       num_indices,
			Rml::TextureHandle                        texture,
			const Rml::Vector2f&                      translation)
		{
			GALAXY_UNUSED(vertices);
			GALAXY_UNUSED(num_vertices);
			GALAXY_UNUSED(indices);
			GALAXY_UNUSED(num_indices);
			GALAXY_UNUSED(texture);
			GALAXY_UNUSED(translation);

			GALAXY_LOG(GALAXY_FATAL, "RenderGeometry() should not be called. Ensure RMLGeometry can be compiled instead.");
		}

		Rml::CompiledGeometryHandle
		RMLRenderer::CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture)
		{
			const auto rml_verts = std::span<Rml::Vertex> {vertices, static_cast<std::size_t>(num_vertices)};
			const auto rml_index = std::span<int> {indices, static_cast<std::size_t>(num_indices)};

			graphics::VertexBuffer        vbo;
			std::vector<graphics::Vertex> vertex_array;

			vertex_array.reserve(rml_verts.size());
			for (auto& vertex : rml_verts)
			{
				graphics::Vertex v;
				v.m_pos.x    = vertex.position.x;
				v.m_pos.y    = vertex.position.y;
				v.m_texels.x = vertex.tex_coord.x;
				v.m_texels.y = vertex.tex_coord.y;
				v.set_colour({vertex.colour.red, vertex.colour.green, vertex.colour.blue, vertex.colour.alpha});

				vertex_array.emplace_back(std::move(v));
			}
			vbo.create(vertex_array, true);

			graphics::IndexBuffer     ibo;
			std::vector<unsigned int> index_array;

			index_array.reserve(rml_index.size());
			for (auto& index : rml_index)
			{
				index_array.push_back(static_cast<unsigned int>(index));
			}
			ibo.create(index_array, true);

			auto rml_vao = new RMLVAO();
			rml_vao->m_vao.create(vbo, ibo);
			rml_vao->m_texture = static_cast<GLuint>(texture);

			return reinterpret_cast<Rml::CompiledGeometryHandle>(rml_vao);
		}

		void RMLRenderer::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation)
		{
			auto rml_vao = reinterpret_cast<RMLVAO*>(geometry);

			m_shader.bind();
			m_shader.set_uniform("u_translation", glm::vec2 {translation.x, translation.y});
			m_shader.set_uniform("u_proj",
				glm::ortho(0.0f, static_cast<float>(SL_HANDLE.window()->get_width()), static_cast<float>(SL_HANDLE.window()->get_height()), 0.0f, -1.0f, 1.0f));

			glViewport(0, 0, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());
			glBindTexture(GL_TEXTURE_2D, rml_vao->m_texture);
			glBindVertexArray(rml_vao->m_vao.id());
			glDrawElements(GL_TRIANGLES, rml_vao->m_vao.index_count(), GL_UNSIGNED_INT, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}

		void RMLRenderer::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
		{
			delete reinterpret_cast<RMLVAO*>(geometry);
		}

		void RMLRenderer::EnableScissorRegion(bool enable)
		{
			if (enable)
			{
				glEnable(GL_SCISSOR_TEST);
			}
			else
			{
				glDisable(GL_SCISSOR_TEST);
			}
		}

		void RMLRenderer::SetScissorRegion(int x, int y, int width, int height)
		{
			glScissor(x, y, width, height);
			m_scissor_region = {x, y, width, height};
		}

		bool RMLRenderer::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
		{
			bool result = true;

			const auto path_opt = SL_HANDLE.vfs()->absolute(source);
			if (path_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find RML texture: {0}.", source);
				result = false;
			}
			else
			{
				stbi_set_flip_vertically_on_load(true);
				unsigned char* data = stbi_load(path_opt.value().c_str(), &texture_dimensions.x, &texture_dimensions.y, nullptr, STBI_rgb_alpha);

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
			}

			return result;
		}

		bool RMLRenderer::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
		{
			GLuint texture = 0;

			glGenTextures(1, &texture);
			if (texture == 0)
			{
				return false;
			}

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
			glGenerateMipmap(GL_TEXTURE_2D);

			if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, SL_HANDLE.config()->get<float>("ansio-filter"));

			glBindTexture(GL_TEXTURE_2D, 0);
			texture_handle = static_cast<Rml::TextureHandle>(texture);

			return true;
		}

		void RMLRenderer::ReleaseTexture(Rml::TextureHandle texture)
		{
			const auto handle = static_cast<GLuint>(texture);
			glDeleteTextures(1, &handle);
		}

		void RMLRenderer::SetTransform(const Rml::Matrix4f* transform)
		{
			if (!transform)
			{
				m_shader.set_uniform("u_transform", m_identity);
			}
			else
			{
				const auto      matrix         = glm::make_mat4(transform->data());
				const glm::vec2 scissor_transf = matrix * glm::vec4 {m_scissor_region.x, m_scissor_region.y, 0.0f, 1.0f};

				SetScissorRegion(static_cast<int>(std::trunc(scissor_transf.x)),
					static_cast<int>(std::trunc(scissor_transf.y)),
					static_cast<int>(std::trunc(m_scissor_region.z)),
					static_cast<int>(std::trunc(m_scissor_region.w)));

				m_shader.bind();
				m_shader.set_uniform("u_transform", matrix);
				m_shader.unbind();
			}
		}
	} // namespace ui
} // namespace galaxy