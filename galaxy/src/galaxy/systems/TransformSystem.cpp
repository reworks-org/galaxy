///
/// TransformSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <optional>

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
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
			world.operate<components::Transform, components::Primitive2D>([&](const ecs::Entity entity,
											  components::Transform* transform,
											  components::Primitive2D* primitive2d) {
				transform->set_rotation_origin(primitive2d->get_width() * 0.5f, primitive2d->get_height() * 0.5f);
			});

			world.operate<components::Transform, components::BatchSprite>([&](const ecs::Entity entity,
											  components::Transform* transform,
											  components::BatchSprite* batchsprite) {
				transform->set_rotation_origin(batchsprite->get_width() * 0.5f, batchsprite->get_height() * 0.5f);
			});

			world.operate<components::Transform, components::Sprite>([&](const ecs::Entity entity,
										     components::Transform* transform,
										     components::Sprite* sprite) {
				transform->set_rotation_origin(sprite->get_width() * 0.5f, sprite->get_height() * 0.5f);
			});
		}
	} // namespace systems
} // namespace galaxy