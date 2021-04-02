///
/// TransformSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <optional>

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Sprite2D.hpp"
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
			world.operate<components::Transform2D, components::BatchSprite>([&](const ecs::Entity entity,
											  components::Transform2D* transform,
											  components::BatchSprite* batchsprite) {
				if (transform->is_dirty())
				{
					transform->set_rotation_origin(batchsprite->get_width() * 0.5f, batchsprite->get_height() * 0.5f);
				}
			});

			world.operate<components::Transform2D, components::Sprite2D>([&](const ecs::Entity entity,
										     components::Transform2D* transform,
										     components::Sprite2D* sprite) {
				if (transform->is_dirty())
				{
					transform->set_rotation_origin(sprite->get_width() * 0.5f, sprite->get_height() * 0.5f);
				}
			});
		}
	} // namespace systems
} // namespace galaxy