///
/// TransformSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Sprite2D.hpp"
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
			world.operate<components::Transform, components::Sprite2D>([&](const ecs::Entity entity,
										       components::Transform* transform,
										       components::Sprite2D* sprite2d) {
				transform->set_rotation_origin(sprite2d->get_width() * 0.5f, sprite2d->get_height() * 0.5f);
			});
		}
	} // namespace systems
} // namespace galaxy