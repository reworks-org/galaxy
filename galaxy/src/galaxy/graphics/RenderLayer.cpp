///
/// RenderLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <execution>

#include "RenderLayer.hpp"

namespace galaxy
{
	namespace graphics
	{
		RenderLayer::RenderLayer(const int layer)
			: m_layer {layer}
		{
			m_data.reserve(50);
		}

		RenderLayer::RenderLayer(RenderLayer&& rl) noexcept
		{
			this->m_batches = std::move(rl.m_batches);
			this->m_data    = std::move(rl.m_data);
			this->m_layer   = rl.m_layer;
		}

		RenderLayer& RenderLayer::operator=(RenderLayer&& rl) noexcept
		{
			if (this != &rl)
			{
				this->m_batches = std::move(rl.m_batches);
				this->m_data    = std::move(rl.m_data);
				this->m_layer   = rl.m_layer;
			}

			return *this;
		}

		RenderLayer::~RenderLayer()
		{
			clear();
		}

		void RenderLayer::submit(Renderable& renderable)
		{
			m_data.emplace_back(std::move(renderable));
		}

		void RenderLayer::submit_batched_sprites(Shader& batch_shader)
		{
			for (auto& [index, batch] : m_batches)
			{
				// clang-format off
				Renderable renderable = {
					.m_vao = batch.vao(),
					.m_texture = batch.gl_texture(),
					.m_index_count = batch.count(),
					.m_type = GL_TRIANGLES,
					.m_configure_shader = [&]()
					{
						batch_shader.bind();
						batch_shader.set_uniform("u_width", static_cast<float>(batch.get_width()));
						batch_shader.set_uniform("u_height", static_cast<float>(batch.get_height()));
					}
				};
				// clang-format on

				submit(renderable);
				batch.buffer_data();
			}
		}

		void RenderLayer::clear()
		{
			for (auto& [index, batch] : m_batches)
			{
				batch.clear();
			}

			m_data.clear();
		}

		void RenderLayer::draw()
		{
			std::sort(std::execution::par, m_data.begin(), m_data.end(), [&](const auto& left, const auto& right) {
				if (left.m_type == right.m_type)
				{
					return left.m_texture < right.m_texture;
				}
				else
				{
					return left.m_type < right.m_type;
				}
			});

			for (auto& renderable : m_data)
			{
				glBindVertexArray(renderable.m_vao);
				glBindTexture(GL_TEXTURE_2D, renderable.m_texture);
				renderable.m_configure_shader();

				if (renderable.m_instance_count > 0)
				{
					glDrawElementsInstanced(renderable.m_type, renderable.m_index_count, GL_UNSIGNED_INT, nullptr, renderable.m_instance_count);
				}
				else
				{
					glDrawElements(renderable.m_type, renderable.m_index_count, GL_UNSIGNED_INT, nullptr);
				}
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}

		const int RenderLayer::get_layer() const noexcept
		{
			return m_layer;
		}
	} // namespace graphics
} // namespace galaxy