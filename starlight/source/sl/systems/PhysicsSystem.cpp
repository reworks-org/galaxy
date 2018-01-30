///
/// PhysicsSystem.cpp
/// starlight
///
///  Created by reworks on 08/11/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <utility>

#include "sl/events/EventTypes.hpp"
#include "sl/physics/Box2DHelper.hpp"
#include "sl/physics/Box2DManager.hpp"
#include "sl/events/CollisionEvent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "PhysicsSystem.hpp"

namespace sl
{
	PhysicsSystem::PhysicsSystem(float ups, int vi, int pi)
	:m_ups(ups), m_velocityIterations(vi), m_positionIterations(pi)
	{
		m_manager = Box2DManager::inst();
	}

	void PhysicsSystem::event(ALLEGRO_EVENT* event)
	{
		switch (event->type)
		{
		case EventTypes::COLLISION_EVENT:
			auto collisionEvent = (CollisionEvent*)event->user.data1;
			auto kvp = m_collisionFunctions.find(std::make_pair(collisionEvent->a, collisionEvent->b));

			if (kvp != m_collisionFunctions.end())
			{
				kvp->second(collisionEvent->a, collisionEvent->b);
			}
			break;
		}
	}

	void PhysicsSystem::update(const double dt, entt::DefaultRegistry& registery)
	{
		m_manager->m_world->Step(1.0f / m_ups, m_velocityIterations, m_positionIterations);

		registery.view<PhysicsComponent, TransformComponent>().each([dt](entt::Entity, PhysicsComponent& pc, TransformComponent& tc)
		{
			tc.m_rect.m_x = b2::metersToPixels<float>(pc.m_body->GetPosition().x);
			tc.m_rect.m_y = b2::metersToPixels<float>(pc.m_body->GetPosition().y);
			tc.m_angle = b2::radToDeg<float>(pc.m_body->GetAngle());
		});
	}
}