///
/// PhysicsSystem.cpp
/// starlight
///
/// Created by reworks on 08/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <utility>

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/events/EventTypes.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/physics/Box2DHelper.hpp"
#include "sl/physics/Box2DManager.hpp"
#include "sl/events/CollisionEvent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "PhysicsSystem.hpp"

namespace sl
{
	PhysicsSystem::PhysicsSystem(const std::string& functionScript, float ups, int vi, int pi)
	:m_ups(ups), m_velocityIterations(vi), m_positionIterations(pi)
	{
		Locator::m_world->m_lua.script(Locator::m_virtualFS->openAsString(functionScript));
		
		sol::table funcs = Locator::m_world->m_lua.get<sol::table>("physicsFuncs");
		funcs.for_each([this](sol::object key, sol::object value)
		{
			sol::table funcTable = value.as<sol::table>();

			std::uint16_t first = funcTable.get<std::uint16_t>("first");
			std::uint16_t second = funcTable.get<std::uint16_t>("second");
			std::string id = funcTable.get<std::string>("id");

			m_collisions.emplace(std::make_pair(first, second), Locator::m_world->m_lua.get<sol::function>(id));

			// Now we need to emplace the reverse in case collision happens the other way.
			m_collisions.emplace(std::make_pair(second, first), Locator::m_world->m_lua.get<sol::function>(id));
		});
	}

	void PhysicsSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
		switch (event->type)
		{
		case EventTypes::COLLISION_EVENT:
			CollisionEvent* ce = (CollisionEvent*)event->user.data1;
			m_collisions[std::make_pair(ce->m_typeA, ce->m_typeB)](ce->m_a, ce->m_b);
			break;
		}
	}

	void PhysicsSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		Locator::m_box2dManager->m_b2world->Step(1.0f / m_ups, m_velocityIterations, m_positionIterations);
		
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