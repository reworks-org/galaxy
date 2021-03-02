///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "RenderSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		void RenderSystem::update(core::World& world, const double dt)
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
						graphics::Renderer2D::m_batch->add(world.get<components::Sprite2D>(entity), transform, renderable->m_z_level);
					}
					else
					{
						m_sorted.emplace_back(data);
					}
				});
			// clang-format on

			if (!graphics::Renderer2D::m_batch->empty())
			{
				graphics::Renderer2D::m_batch->calculate();
			}

			std::sort(std::execution::par, m_sorted.begin(), m_sorted.end(), [&](const auto& left, const auto& right) {
				return left.m_z_level < right.m_z_level;
			});
		}

		void RenderSystem::render(core::World& world, graphics::Camera& camera)
		{
			if (!graphics::Renderer2D::m_batch->empty())
			{
				graphics::Renderer2D::m_batch->calculate();
				graphics::Renderer2D::draw_spritebatch(camera);
			}

			for (const auto& data : m_sorted)
			{
				auto* shader = SL_HANDLE.shaderbook()->get(world.get<components::ShaderID>(data.m_entity)->m_shader_id);
				shader->bind();
				shader->set_uniform("u_cameraProj", camera.get_proj());
				shader->set_uniform("u_cameraView", camera.get_view());

				switch (data.m_type)
				{
					case graphics::Renderables::POINT:
						graphics::Renderer2D::draw_point(world.get<components::Primitive2D>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::LINE:
						graphics::Renderer2D::draw_line(world.get<components::Primitive2D>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::LINE_LOOP:
						graphics::Renderer2D::draw_lineloop(world.get<components::Primitive2D>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::TEXT:
						graphics::Renderer2D::draw_text(world.get<components::Text>(data.m_entity), data.m_transform, shader);
						break;
				}
			}

			m_sorted.clear();
			graphics::Renderer2D::m_batch->clear_sprites();
		}
	} // namespace systems
} // namespace galaxy