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
		{
		}

		RenderSystem2D::~RenderSystem2D() noexcept
		{
		}

		void RenderSystem2D::update(core::Scene2D* scene)
		{
			GALAXY_UNUSED(scene);
		}

		void RenderSystem2D::render(core::World& world)
		{
			world.operate<components::Renderable, components::Transform2D>(
				[&](const ecs::Entity entity, components::Renderable* renderable, components::Transform2D* transform) {
					// Ordered this way for compiler optimizations.
					// Most-Least common.
					switch (renderable->m_type)
					{
						case graphics::Renderables::BATCHED:
							RENDERER_2D().submit(world.get<components::BatchSprite>(entity), transform);
							break;

						case graphics::Renderables::SPRITE:
							RENDERER_2D().submit(world.get<components::Sprite>(entity), transform);
							break;

						case graphics::Renderables::PARTICLE:
							RENDERER_2D().submit(world.get<components::ParticleEffect>(entity));
							break;

						case graphics::Renderables::TEXT:
							RENDERER_2D().submit(world.get<components::Text>(entity), transform);
							break;

						case graphics::Renderables::LINE_LOOP:
							RENDERER_2D().submit(world.get<components::Primitive2D>(entity), transform);
							break;

						case graphics::Renderables::LINE:
							RENDERER_2D().submit(world.get<components::Primitive2D>(entity), transform);
							break;

						case graphics::Renderables::POINT:
							RENDERER_2D().submit(world.get<components::Primitive2D>(entity), transform);
							break;
					}
				});
		}
	} // namespace systems
} // namespace galaxy