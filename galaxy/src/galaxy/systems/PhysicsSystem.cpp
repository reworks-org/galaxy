///
/// PhysicsSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/trigonometric.hpp>

#include "galaxy/components/Transform.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/scene/Scene.hpp"

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

		void PhysicsSystem::update(scene::Scene* scene)
		{
			const auto view = scene->m_world.m_registry.group<components::RigidBody>(entt::get<components::Transform>, entt::exclude<flags::Disabled>);
			for (auto&& [entity, body, transform] : view.each())
			{
				const auto& pos = body.m_body->GetPosition();
				transform.set_pos(pos.x * GALAXY_BOX_TO_WORLD, pos.y * GALAXY_BOX_TO_WORLD);
				transform.set_rotation(glm::degrees(body.m_body->GetAngle()));
			}
		}
	} // namespace systems
} // namespace galaxy
