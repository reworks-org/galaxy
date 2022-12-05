///
/// PhysicsSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/trigonometric.hpp>

#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/physics/Constants.hpp"
#include "galaxy/scene/Scene.hpp"

#include "PhysicsSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		PhysicsSystem::PhysicsSystem() noexcept
		{
		}

		PhysicsSystem::~PhysicsSystem() noexcept
		{
		}

		void PhysicsSystem::update(scene::Scene* scene)
		{
			const auto view = scene->m_world.m_registry.view<components::RigidBody, components::Transform, components::Flag>();
			for (auto&& [entity, body, transform, flag] : view.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					if (!body.m_body->IsEnabled())
					{
						body.m_body->SetEnabled(true);
					}

					const auto& pos = body.m_body->GetPosition();
					transform.set_pos(pos.x * physics::Constants::pixels_per_meter, pos.y * physics::Constants::pixels_per_meter);
					transform.set_rotation(glm::degrees(body.m_body->GetAngle()));
				}
				else
				{
					body.m_body->SetEnabled(false);
				}
			}
		}
	} // namespace systems
} // namespace galaxy