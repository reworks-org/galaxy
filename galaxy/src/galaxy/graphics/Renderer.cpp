///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <execution>

#include "galaxy/utils/Globals.hpp"

#include "Renderer.hpp"

namespace
{
	///
	/// Render To Texture vertex shader.
	///
	constexpr const char* const render_to_texture_vert = R"(
		#version 460 core
		layout(location = 0) in vec2 l_pos;
		layout(location = 1) in vec2 l_texels;
		layout(location = 2) in vec4 l_colour;

		out vec2 io_texels;
	
		uniform mat4 u_projection;
		uniform mat4 u_transform;

		void main()
		{
			io_texels = l_texels;

			gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);
		}
	)";

	///
	/// Render To Texture fragment shader.
	///
	constexpr const char* const render_to_texture_frag = R"(
		#version 460 core

		in vec2 io_texels;
		out vec4 io_frag_colour;

		uniform sampler2D u_texture;

		void main()
		{
			io_frag_colour = texture(u_texture, io_texels);
		}
	)";
} // namespace

namespace galaxy
{
	namespace graphics
	{
		std::unique_ptr<UniformBuffer> Renderer::s_ubo = nullptr;
		std::vector<Renderable*> Renderer::s_data;
		std::unique_ptr<Shader> Renderer::s_rtt_shader = nullptr;

		void Renderer::init() noexcept
		{
			s_ubo = std::make_unique<UniformBuffer>();
			s_ubo->create(GAlAXY_CAMERA_UBO_INDEX);
			s_ubo->reserve(sizeof(Camera::Data));

			s_data.reserve(GALAXY_DEFAULT_RENDERER_RESERVED);

			s_rtt_shader = std::make_unique<Shader>();
			s_rtt_shader->load_raw(render_to_texture_vert, render_to_texture_frag);
		}

		void Renderer::destroy() noexcept
		{
			s_data.clear();

			s_ubo.reset();
			s_rtt_shader.reset();

			s_ubo        = nullptr;
			s_rtt_shader = nullptr;
		}

		void Renderer::buffer_camera(Camera& camera) noexcept
		{
			s_ubo->sub_buffer<Camera::Data>(0, 1, &camera.get_data());
		}

		void Renderer::submit(Renderable* renderable)
		{
			s_data.push_back(renderable);
		}

		void Renderer::draw()
		{
			std::sort(s_data.begin(), s_data.end(), [](Renderable* left, Renderable* right) {
				if (left->m_layer == right->m_layer)
				{
					if (left->m_type == right->m_type)
					{
						return left->m_texture < right->m_texture;
					}
					else
					{
						return left->m_type < right->m_type;
					}
				}
				else
				{
					return left->m_layer < right->m_layer;
				}
			});

			for (auto i = 0; i < s_data.size(); i++)
			{
				auto renderable = s_data[i];

				glBindVertexArray(renderable->m_vao);
				glBindTexture(GL_TEXTURE_2D, renderable->m_texture);

				renderable->configure_shader();

				glDrawElementsInstanced(renderable->m_type, renderable->m_count, GL_UNSIGNED_INT, nullptr, renderable->m_instances);
			}
		}

		void Renderer::draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform)
		{
			va.bind();
			texture.bind();

			s_rtt_shader->bind();
			s_rtt_shader->set_uniform("u_projection", target.get_proj());
			s_rtt_shader->set_uniform("u_transform", transform.get_transform());

			glDrawElements(GL_TRIANGLES, va.index_count(), GL_UNSIGNED_INT, nullptr);
		}
	} // namespace graphics
} // namespace galaxy