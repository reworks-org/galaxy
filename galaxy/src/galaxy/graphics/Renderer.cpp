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

namespace galaxy
{
	namespace graphics
	{
		void Renderer::init() noexcept
		{
			m_ubo.create(GAlAXY_CAMERA_UBO_INDEX);
			m_ubo.reserve(sizeof(Camera::Data));

			m_data.reserve(GALAXY_DEFAULT_RENDERER_RESERVED);
		}

		void Renderer::destroy() noexcept
		{
			m_ubo.reset();
			m_data.clear();
		}

		void Renderer::buffer_camera(Camera& camera)
		{
			m_ubo.sub_buffer<Camera::Data>(0, 1, &camera.get_data());
		}

		void Renderer::submit(Renderable* renderable)
		{
			m_data.push_back(renderable);
		}

		void Renderer::draw()
		{
			std::sort(m_data.begin(), m_data.end(), [](Renderable* left, Renderable* right) {
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

			for (auto i = 0; i < m_data.size(); i++)
			{
				auto renderable = m_data[i];

				glBindVertexArray(renderable->m_vao);
				glBindTexture(GL_TEXTURE_2D, renderable->m_texture);

				renderable->configure_shader();

				glDrawElementsInstanced(renderable->m_type, renderable->m_count, GL_UNSIGNED_INT, nullptr, renderable->m_instances);
			}
		}
	} // namespace graphics
} // namespace galaxy