//
//  PhysicsSystem.cpp
//  rework
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/physics/Box2DHelper.hpp"
#include "re/managers/Box2DManager.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/PhysicsComponent.hpp"
#include "re/components/TransformComponent.hpp"

#include "PhysicsSystem.hpp"

namespace re
{
	PhysicsSystem::PhysicsSystem(float ups, int32 vi, int32 pi)
	:m_ups(ups), m_velocityIterations(vi), m_positionIterations(pi)
	{
		m_manager = Locator::get<Box2DManager>();
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}

	void PhysicsSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
	{
		m_manager->world()->Step(1.0f / m_ups, m_velocityIterations, m_positionIterations);

		es.each<PhysicsComponent, TransformComponent>([](entityx::Entity& e, PhysicsComponent& pc, TransformComponent& tc)
		{
			tc.m_rect.x = b2::metersToPixels<float>(pc.m_body->GetPosition().x);
			tc.m_rect.y = b2::metersToPixels<float>(pc.m_body->GetPosition().y);
			tc.m_angle = b2::radToDeg<float>(pc.m_body->GetAngle());
		});
	}
}