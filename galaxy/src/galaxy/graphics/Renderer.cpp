///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

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
		int Renderer::s_prev_shader  = -1;
		int Renderer::s_prev_texture = -1;

		void Renderer::init()
		{
			s_ubo = std::make_unique<UniformBuffer>();
			s_ubo->create(GAlAXY_CAMERA_UBO_INDEX);
			s_ubo->reserve(sizeof(Camera::Data));

			s_data.reserve(GALAXY_DEFAULT_RENDERER_RESERVED);
		}

		void Renderer::destroy()
		{
			s_data.clear();

			s_ubo.reset();
			s_ubo = nullptr;

			s_prev_shader  = -1;
			s_prev_texture = -1;
		}

		void Renderer::buffer_camera(Camera& camera)
		{
			s_ubo->sub_buffer<Camera::Data>(0, 1, &camera.get_data());
		}

		void Renderer::submit(Renderable* renderable)
		{
			s_data.push_back(renderable);
		}

		void Renderer::draw()
		{
			s_prev_shader  = -1;
			s_prev_texture = -1;

			// I don't think we would ever sort the amount of renderable data required to do this in parallel faster.
			// std::execution::par maybe used when > 1000? But 1000 draw calls is too slow anyway.

			std::sort(s_data.begin(), s_data.end(), [](const Renderable* left, const Renderable* right) -> bool {
				if (left->m_layer == right->m_layer)
				{
					if (left->m_texture_id == right->m_texture_id)
					{
						return left->m_shader_sort_id < right->m_shader_sort_id;
					}
					else
					{
						return left->m_texture_id < right->m_texture_id;
					}
				}
				else
				{
					return left->m_layer < right->m_layer;
				}
			});

			glActiveTexture(GL_TEXTURE0);

			for (auto i = 0; i < s_data.size(); i++)
			{
				auto renderable = s_data[i];

				const auto shader = renderable->m_configure_shader();
				if (s_prev_shader != shader)
				{
					glUseProgram(shader);
					s_prev_shader = shader;
				}

				// Don't need to check, usually will always be different.
				glBindVertexArray(renderable->m_vao_id);

				if (s_prev_texture != renderable->m_texture_id)
				{
					glBindTexture(GL_TEXTURE_2D, renderable->m_texture_id);
					s_prev_texture = renderable->m_texture_id;
				}

				// Instances = 1 is the same as glDrawElements.
				glDrawElementsInstanced(renderable->m_type, renderable->m_index_count, GL_UNSIGNED_INT, nullptr, renderable->m_instances);
			}
		}

		void Renderer::flush()
		{
			s_data.clear();
		}

		void Renderer::draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform)
		{
			glActiveTexture(GL_TEXTURE0);

			va.bind();
			texture.bind();

			auto rtt = core::ServiceLocator<resource::Shaders>::ref().get("RenderToTexture");
			if (rtt != nullptr)
			{
				rtt->bind();
				rtt->set_uniform("u_projection", target.get_proj());
				rtt->set_uniform("u_transform", transform.get_transform());

				glDrawElements(GL_TRIANGLES, va.index_count(), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to fetch default draw_texture_to_target shader.");
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}

		void Renderer::draw_texture_to_framebuffer(const unsigned int texture, VertexArray& va, components::Transform& transform, const glm::mat4& proj)
		{
			va.bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			auto rtt = core::ServiceLocator<resource::Shaders>::ref().get("RenderToTexture");
			if (rtt != nullptr)
			{
				rtt->bind();
				rtt->set_uniform("u_projection", proj);
				rtt->set_uniform("u_transform", transform.get_transform());

				glDrawElements(GL_TRIANGLES, va.index_count(), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to fetch default draw_texture_to_framebuffer shader.");
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}

		std::vector<Renderable*>& Renderer::get_data()
		{
			return s_data;
		}
	} // namespace graphics
} // namespace galaxy