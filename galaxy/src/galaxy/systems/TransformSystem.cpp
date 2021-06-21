///
/// TransformSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/core/World.hpp"

#include "TransformSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		TransformSystem::TransformSystem() noexcept
		{
		}

		TransformSystem::~TransformSystem() noexcept
		{
		}

		void TransformSystem::update(core::World& world, const double dt)
		{
			world.operate<components::Transform2D, components::BatchSprite>(std::execution::par, [&](const ecs::Entity entity, components::Transform2D* transform, components::BatchSprite* batchsprite) {
				if (transform->is_dirty())
				{
					const auto& region = batchsprite->get_region();
					transform->set_origin(region.m_width * 0.5f, region.m_height * 0.5f);
				}
			});

			world.operate<components::Transform2D, components::Sprite>(std::execution::par, [&](const ecs::Entity entity, components::Transform2D* transform, components::Sprite* sprite) {
				if (transform->is_dirty())
				{
					transform->set_origin(sprite->get_width() * 0.5f, sprite->get_height() * 0.5f);
				}
			});
		}
	} // namespace systems
} // namespace galaxy