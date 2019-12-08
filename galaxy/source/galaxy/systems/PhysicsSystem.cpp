///
/// PhysicsSystem.cpp
/// galaxy
///
/// Created by reworks on 08/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <utility>

#include "galaxy/fs/VirtualFS.hpp"
#include "galaxy/libs/loguru/loguru.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/physics/Box2DHelper.hpp"
#include "galaxy/libs/entt/signal/dispatcher.hpp"
#include "galaxy/components/EnabledComponent.hpp"
#include "galaxy/components/PhysicsComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"

#include "PhysicsSystem.hpp"

namespace galaxy
{
	PhysicsSystem::PhysicsSystem(const std::string& functionScript, float ups, int vi, int pi)
	:m_ups(ups), m_step(0), m_velocityIterations(vi), m_positionIterations(pi)
	{
		if (functionScript != "")
		{
			// Process collision functions.
			Locator::lua->script(Locator::virtualFS->openAsString(functionScript));

			// For each function in the script...
			sol::table funcs = Locator::lua->get<sol::table>("physicsFuncs");
			if (!funcs.empty())
			{
				funcs.for_each([this](sol::object key, sol::object value)
				{
					// ...get function information and add it to the collision map...
					sol::table funcTable = value.as<sol::table>();

					std::uint16_t first = funcTable.get<std::uint16_t>("first");
					std::uint16_t second = funcTable.get<std::uint16_t>("second");
					std::string id = funcTable.get<std::string>("id");

					// ...and then add the function to sol2.
					m_collisions.emplace(std::make_pair(first, second), Locator::lua->get<sol::function>(id));

					// Now we need to emplace the reverse in case collision happens the other way.
					m_collisions.emplace(std::make_pair(second, first), Locator::lua->get<sol::function>(id));
				});
			}
			else
			{
				LOG_S(WARNING) << "No collision functions found for script: " << functionScript;
			}
		}
		else
		{
			LOG_S(WARNING) << "No collision function script for PhysicsSystem!";
		}

		// Attach collision listener.
		Locator::dispatcher->sink<CollisionEvent>().connect(this);

		// Calc step.
		m_step = 1.0f / m_ups;
	}

	void PhysicsSystem::receive(const CollisionEvent& ce)
	{
		// When a collision event happens, call the correct collision function using information from collision event.
		m_collisions[std::make_pair(ce.m_typeA, ce.m_typeB)](ce.m_a, ce.m_b);
	}

	void PhysicsSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		// Update Box2D world.
		Locator::box2dHelper->m_b2world->Step(m_step, m_velocityIterations, m_positionIterations);
			
		// Iterate over entities, updating their transformcomponent to match the physics component.
		registry.view<PhysicsComponent, TransformComponent, EnabledComponent>()
			.each([&](entt::DefaultRegistry::entity_type entity, PhysicsComponent& pc, TransformComponent& tc, EnabledComponent& ec)
		{
			// Make sure physics body is valid.
			if (pc.m_body != nullptr)
			{
				tc.m_rect.m_x = Box2DHelper::metersToPixels<float>(pc.m_body->GetPosition().x);
				tc.m_rect.m_y = Box2DHelper::metersToPixels<float>(pc.m_body->GetPosition().y);
				tc.m_angle = Box2DHelper::radToDeg<float>(pc.m_body->GetAngle());
			}
			else
			{
				LOG_S(ERROR) << "PhysicsComponent body was nullptr!";
			}
		});
	}
}