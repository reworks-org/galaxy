///
/// PhysicsSystem.cpp
/// starlight
///
/// Created by reworks on 08/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
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

	void PhysicsSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
		switch (event->type)
		{
		case EventTypes::COLLISION_EVENT:
			auto collisionEvent = (CollisionEvent*)event->user.data1;
			auto kvp = m_collisionFunctions.find(std::make_pair(collisionEvent->m_entityA, collisionEvent->m_entityB));

			if (kvp != m_collisionFunctions.end())
			{
				kvp->second(collisionEvent->m_entityA, collisionEvent->m_entityB);
			}
			break;
		}
	}

	void PhysicsSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		m_manager->m_world->Step(1.0f / m_ups, m_velocityIterations, m_positionIterations);
		
		auto view = registry.view<PhysicsComponent, TransformComponent>();
		for (entt::Entity entity : view)
		{
			PhysicsComponent& pc = view.get<PhysicsComponent>(entity);
			TransformComponent& tc = view.get<TransformComponent>(entity);

			tc.m_rect.m_x = b2::metersToPixels<float>(pc.m_body->GetPosition().x);
			tc.m_rect.m_y = b2::metersToPixels<float>(pc.m_body->GetPosition().y);
			tc.m_angle = b2::radToDeg<float>(pc.m_body->GetAngle());
		}
	}
}