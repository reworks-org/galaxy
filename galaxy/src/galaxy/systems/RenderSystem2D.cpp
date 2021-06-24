///
/// RenderSystem2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/TextureBook.hpp"

#include "RenderSystem2D.hpp"

namespace galaxy
{
	namespace systems
	{
		void RenderSystem2D::update(core::World& world, const double dt)
		{
			m_points.clear();
			m_lines.clear();
			m_lineloops.clear();
			m_text.clear();
			m_sprites.clear();
			m_batchsprites.clear();
			SL_HANDLE.texturebook()->clear_sprites();

			world.operate<components::Renderable, components::Transform2D>(std::execution::par, [&](const ecs::Entity entity, components::Renderable* renderable, components::Transform2D* transform) {
				// Ordered this way for compiler optimizations.
				// Most-Least common.
				switch (renderable->m_type)
				{
					case graphics::Renderables::BATCHED:
						m_batch_lock.lock();
						m_batchsprites.emplace_back(world.get<components::BatchSprite>(entity), transform);
						m_batch_lock.unlock();
						break;

					case graphics::Renderables::SPRITE:
						m_sprite_lock.lock();
						m_sprites.emplace_back(world.get<components::Sprite>(entity), transform);
						m_sprite_lock.unlock();
						break;

					case graphics::Renderables::TEXT:
						m_text_lock.lock();
						m_text.emplace_back(world.get<components::Text>(entity), transform);
						m_text_lock.unlock();
						break;

					case graphics::Renderables::LINE_LOOP:
						m_lineloop_lock.lock();
						m_lineloops.emplace_back(world.get<components::Primitive2D>(entity), transform);
						m_lineloop_lock.unlock();
						break;

					case graphics::Renderables::LINE:
						m_line_lock.lock();
						m_lines.emplace_back(world.get<components::Primitive2D>(entity), transform);
						m_line_lock.unlock();
						break;

					case graphics::Renderables::POINT:
						m_point_lock.lock();
						m_points.emplace_back(world.get<components::Primitive2D>(entity), transform);
						m_point_lock.unlock();
						break;
				}
			});

			for (const auto& [batch, transform] : m_batchsprites)
			{
				SL_HANDLE.texturebook()->add(batch, transform);
			}

			SL_HANDLE.texturebook()->buffer_spritebatch_data();
		}

		void RenderSystem2D::render(core::World& world, graphics::Camera2D& camera)
		{
			RENDERER_2D().bind_sb_shader();
			for (auto& [index, batch] : SL_HANDLE.texturebook()->get_spritebatches())
			{
				RENDERER_2D().draw_spritebatch(&batch);
			}

			RENDERER_2D().bind_point();
			for (const auto& [point, transform] : m_points)
			{
				RENDERER_2D().draw_point(point, transform);
			}

			RENDERER_2D().bind_line();
			for (const auto& [line, transform] : m_lines)
			{
				RENDERER_2D().draw_line(line, transform);
			}

			// Dont need to rebind line shader here.
			for (const auto& [lineloop, transform] : m_lineloops)
			{
				RENDERER_2D().draw_lineloop(lineloop, transform);
			}

			RENDERER_2D().bind_text();
			for (const auto& [text, transform] : m_text)
			{
				RENDERER_2D().draw_text(text, transform);
			}

			RENDERER_2D().bind_sprite();
			for (const auto& [sprite, transform] : m_sprites)
			{
				RENDERER_2D().draw_sprite(sprite, transform);
			}
		}
	} // namespace systems
} // namespace galaxy