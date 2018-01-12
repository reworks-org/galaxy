///
/// PhysicsSystem.cpp
/// starlight
///
///  Created by reworks on 08/11/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "re/physics/Box2DHelper.hpp"
#include "re/physics/Box2DManager.hpp"
#include "re/components/PhysicsComponent.hpp"
#include "re/components/TransformComponent.hpp"

#include "PhysicsSystem.hpp"

namespace sl
{
	PhysicsSystem::PhysicsSystem(float ups, int vi, int pi)
	:m_ups(ups), m_velocityIterations(vi), m_positionIterations(pi)
	{
		m_manager = Box2DManager::get();
	}

	void PhysicsSystem::update(const double dt, entt::DefaultRegistry& registery)
	{
		m_manager->m_world->Step(1.0f / m_ups, m_velocityIterations, m_positionIterations);

		registery.view<PhysicsComponent, TransformComponent>().each([dt](std::uint32_t entity, PhysicsComponent& pc, TransformComponent& tc)
		{
			tc.m_rect.m_x = b2::metersToPixels<float>(pc.m_body->GetPosition().x);
			tc.m_rect.m_y = b2::metersToPixels<float>(pc.m_body->GetPosition().y);
			tc.m_angle = b2::radToDeg<float>(pc.m_body->GetAngle());
		});
	}
}