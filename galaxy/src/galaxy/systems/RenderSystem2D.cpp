///
/// RenderSystem2D.cpp
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

#include "RenderSystem2D.hpp"

namespace galaxy
{
	namespace systems
	{
		void RenderSystem2D::update(core::World& world, const double dt)
		{
			m_sorted.clear();
			RENDERER_2D().clear();

			// clang-format off
			world.operate<components::Renderable, components::Transform2D>([&](const ecs::Entity entity, components::Renderable* renderable, components::Transform2D* transform) -> void
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
						RENDERER_2D().add_batched_sprite(world.get<components::BatchSprite>(entity), transform, renderable->m_z_level);
					}
					else
					{
						m_sorted.emplace_back(data);
					}
				});
			// clang-format on

			RENDERER_2D().calculate_batches();
			std::sort(std::execution::par, m_sorted.begin(), m_sorted.end(), [&](const auto& left, const auto& right) {
				return left.m_z_level < right.m_z_level;
			});
		}

		void RenderSystem2D::render(core::World& world, graphics::OrthoCamera& camera)
		{
			RENDERER_2D().calculate_batches();
			RENDERER_2D().draw_spritebatches(camera);

			for (const auto& data : m_sorted)
			{
				auto* shader = SL_HANDLE.shaderbook()->get(world.get<components::ShaderID>(data.m_entity)->m_shader_id);
				shader->bind();
				shader->set_uniform("u_cameraProj", camera.get_proj());
				shader->set_uniform("u_cameraView", camera.get_view());

				switch (data.m_type)
				{
					case graphics::Renderables::POINT:
						RENDERER_2D().draw_point(world.get<components::Primitive2D>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::LINE:
						RENDERER_2D().draw_line(world.get<components::Primitive2D>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::LINE_LOOP:
						RENDERER_2D().draw_lineloop(world.get<components::Primitive2D>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::TEXT:
						RENDERER_2D().draw_text(world.get<components::Text>(data.m_entity), data.m_transform, shader);
						break;

					case graphics::Renderables::SPRITE:
						RENDERER_2D().draw_sprite(world.get<components::Sprite2D>(data.m_entity), data.m_transform, shader);
						break;
				}
			}
		}
	} // namespace systems
} // namespace galaxy