///
/// PhysicsSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Physics.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/World.hpp"

#include "PhysicsSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		PhysicsSystem::PhysicsSystem()
		{
		}

		PhysicsSystem::~PhysicsSystem()
		{
		}

		void PhysicsSystem::update(core::World& world, const double dt)
		{
			world.operate<components::Physics, components::Transform>([&](const ecs::Entity entity, components::Physics* physics, components::Transform* transform) {
				const auto pos = physics->body()->GetPosition();

				transform->set_pos(pos.x * physics::FROM_METERS_TO_PIXELS, pos.y * physics::FROM_METERS_TO_PIXELS);
				transform->rotate(glm::degrees(physics->body()->GetAngle()));
			});
		}
	} // namespace systems
} // namespace galaxy