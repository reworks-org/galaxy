///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Line.hpp"
#include "galaxy/components/Point.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		void RenderSystem::update(const double dt)
		{
		}

		void RenderSystem::render(core::World& world, graphics::Camera& camera)
		{
			// clang-format off
			world.operate<components::Renderable, components::Transform>([&](const ecs::Entity entity, components::Renderable* renderable, components::Transform* transform) -> void
			{
					RenderData data
					{
						.m_entity = entity,
						.m_type = renderable->m_type,
						.m_z_level = renderable->m_z_level,
						.m_transform = transform
					};
					
					if (renderable->m_type == graphics::Renderables::BATCHED)
					{
						graphics::Renderer::m_batch->add(world.get<components::BatchedSprite>(entity), transform, renderable->m_z_level);
					}
					else
					{
						m_sorted.emplace_back(data);
					}
			});
			// clang-format on

			if (!graphics::Renderer::m_batch->empty())
			{
				graphics::Renderer::m_batch->calculate();
				graphics::Renderer::submit_batch(camera);
			}

			std::sort(std::execution::par, m_sorted.begin(), m_sorted.end(), [&](const auto& left, const auto& right) {
				return left.m_z_level > right.m_z_level;
			});

			for (const auto& data : m_sorted)
			{
				auto* shader = SL_HANDLE.shaderbook()->get(world.get<components::ShaderID>(data.m_entity)->m_shader_id);
				shader->bind();
				shader->set_uniform("u_cameraProj", camera.get_proj());
				shader->set_uniform("u_cameraView", camera.get_transform());

				switch (data.m_type)
				{
					case graphics::Renderables::POINT:
						graphics::Renderer::submit_point(world.get<components::Point>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::LINE:
						graphics::Renderer::submit_line(world.get<components::Line>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::CIRCLE:
						graphics::Renderer::submit_circle(world.get<components::Circle>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::SPRITE:
						graphics::Renderer::submit_sprite(world.get<components::Sprite>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::TEXT:
						graphics::Renderer::submit_text(world.get<components::Text>(data.m_entity), data.m_transform, shader);
						break;
				}
			}

			m_sorted.clear();
		}
	} // namespace systems
} // namespace galaxy