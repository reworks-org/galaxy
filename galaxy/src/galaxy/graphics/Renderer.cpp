///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/utils/Globals.hpp"

#include "Renderer.hpp"

namespace galaxy
{
	namespace graphics
	{
		std::unique_ptr<UniformBuffer> Renderer::s_ubo = nullptr;
		std::vector<Renderable*> Renderer::s_data;
		int Renderer::s_prev_shader  = 0;
		int Renderer::s_prev_texture = 0;

		void Renderer::init() noexcept
		{
			s_ubo = std::make_unique<UniformBuffer>();
			s_ubo->create(GAlAXY_CAMERA_UBO_INDEX);
			s_ubo->reserve(sizeof(Camera::Data));

			s_data.reserve(GALAXY_DEFAULT_RENDERER_RESERVED);
		}

		void Renderer::destroy() noexcept
		{
			s_data.clear();

			s_ubo.reset();
			s_ubo = nullptr;
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
			std::sort(s_data.begin(), s_data.end(), [](const Renderable* left, const Renderable* right) noexcept -> bool {
				if (left->m_layer == right->m_layer)
				{
					if (left->m_texture == right->m_texture)
					{
						return left->m_shader < right->m_shader;
					}
					else
					{
						return left->m_texture < right->m_texture;
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

				if (s_prev_shader != renderable->m_shader)
				{
					glUseProgram(renderable->m_shader);
					s_prev_shader = renderable->m_shader;
				}

				// Don't need to check, usually will always be different.
				glBindVertexArray(renderable->m_vao);

				if (s_prev_texture != renderable->m_texture)
				{
					glBindTexture(GL_TEXTURE_2D, renderable->m_texture);
					s_prev_texture = renderable->m_texture;
				}

				// Instances = 1 is the same as glDrawElements.
				glDrawElementsInstanced(renderable->m_type, renderable->m_count, GL_UNSIGNED_INT, nullptr, renderable->m_instances);
			}

			s_data.clear();
		}

		void Renderer::draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform)
		{
			va.bind();
			texture.bind();

			auto& shaders = core::ServiceLocator<resource::Shaders>::ref();
			auto rtt      = shaders.get("RenderToTexture");
			if (rtt != nullptr)
			{
				rtt->bind();
				rtt->set_uniform("u_projection", target.get_proj());
				rtt->set_uniform("u_transform", transform.get_transform());

				glDrawElements(GL_TRIANGLES, va.index_count(), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to fetch default render to texture shader.");
			}
		}
	} // namespace graphics
} // namespace galaxy