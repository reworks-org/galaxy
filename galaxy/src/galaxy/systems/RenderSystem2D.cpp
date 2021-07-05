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
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/TextureBook.hpp"

#include "RenderSystem2D.hpp"

namespace galaxy
{
	namespace systems
	{
		RenderSystem2D::RenderSystem2D() noexcept
		    : m_quadtree {0, {0, 0, 0, 0}}
		{
		}

		RenderSystem2D::~RenderSystem2D() noexcept
		{
			m_points.clear();
			m_lines.clear();
			m_lineloops.clear();
			m_text.clear();
			m_sprites.clear();
			SL_HANDLE.texturebook()->clear_sprites();
			m_quadtree.clear();
			m_output.clear();
		}

		void RenderSystem2D::update(core::Scene2D* scene, const double dt)
		{
			m_points.clear();
			m_lines.clear();
			m_lineloops.clear();
			m_text.clear();
			m_sprites.clear();
			SL_HANDLE.texturebook()->clear_sprites();
			m_quadtree.clear();
			m_output.clear();

			if (!scene->get_active_map())
			{
				m_quadtree.resize(SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());
			}
			else
			{
				m_quadtree.resize(scene->get_active_map()->get_width(), scene->get_active_map()->get_height());
			}

			scene->m_world.operate<components::Renderable>([&](const ecs::Entity entity, components::Renderable* renderable) {
				m_quadtree.insert({.m_aabb = &renderable->get_aabb(), .m_entity = entity});
			});

			m_quadtree.query({.m_aabb = &scene->m_camera.get_aabb(), .m_entity = 0}, m_output);
			for (auto* object : m_output)
			{
				auto* renderable = scene->m_world.get<components::Renderable>(object->m_entity);
				auto* transform  = scene->m_world.get<components::Transform2D>(object->m_entity);

				// Ordered this way for compiler optimizations.
				// Most-Least common.
				switch (renderable->m_type)
				{
					case graphics::Renderables::BATCHED:
						SL_HANDLE.texturebook()->add(scene->m_world.get<components::BatchSprite>(object->m_entity), transform);
						break;

					case graphics::Renderables::SPRITE:
						m_sprites.emplace_back(scene->m_world.get<components::Sprite>(object->m_entity), transform);
						break;

					case graphics::Renderables::TEXT:
						m_text.emplace_back(scene->m_world.get<components::Text>(object->m_entity), transform);
						break;

					case graphics::Renderables::LINE_LOOP:
						m_lineloops.emplace_back(scene->m_world.get<components::Primitive2D>(object->m_entity), transform);
						break;

					case graphics::Renderables::LINE:
						m_lines.emplace_back(scene->m_world.get<components::Primitive2D>(object->m_entity), transform);
						break;

					case graphics::Renderables::POINT:
						m_points.emplace_back(scene->m_world.get<components::Primitive2D>(object->m_entity), transform);
						break;
				}
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