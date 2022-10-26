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
#include "galaxy/state/Layer.hpp"
#include "galaxy/utils/Globals.hpp"

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

		void PhysicsSystem::update(state::Layer* layer)
		{
			const auto view = layer->world().m_registry.view<components::RigidBody, components::Transform, components::Flag>();
			for (auto&& [entity, body, transform, flag] : view.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					const auto& pos = body.m_body->GetPosition();

					transform.set_pos(pos.x, pos.y);
					transform.set_rotation(glm::radians(body.m_body->GetAngle()));
				}
			}
		}
	} // namespace systems
} // namespace galaxy