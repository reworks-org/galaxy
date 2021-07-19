///
/// RenderSystem2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/ParticleEffect.hpp"
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
			m_quadtree.clear();
			m_output.clear();
		}

		void RenderSystem2D::update(core::Scene2D* scene, const double dt)
		{
			m_quadtree.clear();
			m_output.clear();

			m_quadtree.resize(SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());
			scene->m_world.operate<components::Renderable>([&](const ecs::Entity entity, components::Renderable* renderable) {
				m_quadtree.insert({.m_aabb = &renderable->get_aabb(), .m_entity = entity, .m_type = renderable->m_type});
			});

			m_quadtree.query({.m_aabb = &scene->m_camera.get_aabb()}, m_output);
		}

		void RenderSystem2D::render(core::World& world, graphics::Camera2D& camera)
		{
			for (auto* object : m_output)
			{
				// Ordered this way for compiler optimizations.
				// Most-Least common.
				switch (object->m_type)
				{
					case graphics::Renderables::BATCHED:
						RENDERER_2D().submit(world.get<components::BatchSprite>(object->m_entity), world.get<components::Transform2D>(object->m_entity));
						break;

					case graphics::Renderables::SPRITE:
						RENDERER_2D().submit(world.get<components::Sprite>(object->m_entity), world.get<components::Transform2D>(object->m_entity));
						break;

					case graphics::Renderables::PARTICLE:
						RENDERER_2D().submit(world.get<components::ParticleEffect>(object->m_entity));
						break;

					case graphics::Renderables::TEXT:
						RENDERER_2D().submit(world.get<components::Text>(object->m_entity), world.get<components::Transform2D>(object->m_entity));
						break;

					case graphics::Renderables::LINE_LOOP:
						RENDERER_2D().submit(world.get<components::Primitive2D>(object->m_entity), world.get<components::Transform2D>(object->m_entity));
						break;

					case graphics::Renderables::LINE:
						RENDERER_2D().submit(world.get<components::Primitive2D>(object->m_entity), world.get<components::Transform2D>(object->m_entity));
						break;

					case graphics::Renderables::POINT:
						RENDERER_2D().submit(world.get<components::Primitive2D>(object->m_entity), world.get<components::Transform2D>(object->m_entity));
						break;
				}
			}
		}
	} // namespace systems
} // namespace galaxy