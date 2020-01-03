///
/// Box2DCallbacks.cpp
/// galaxy
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/libs/loguru/loguru.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/events/CollisionEvent.hpp"
#include "galaxy/libs/entt/signal/dispatcher.hpp"
#include "galaxy/libs/Box2D/Dynamics/Contacts/b2Contact.h"

#include "Box2DCallbacks.hpp"

namespace galaxy
{
	void CollisionContact::BeginContact(b2Contact* contact)
	{
		// Get colliding fixtures from contact.
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		if ((fixtureA) && (fixtureB))
		{
			// Retrieve the entity the fixtures belong to.
			entt::DefaultRegistry::entity_type a = *static_cast<entt::DefaultRegistry::entity_type*>(fixtureA->GetUserData());
			entt::DefaultRegistry::entity_type b = *static_cast<entt::DefaultRegistry::entity_type*>(fixtureB->GetUserData());

			// Emit the collision event based off of the info provided.
			Locator::dispatcher->trigger<CollisionEvent>(a, b, fixtureA->GetFilterData().categoryBits, fixtureB->GetFilterData().categoryBits);
		}
		else
		{
			LOG_S(WARNING) << "NULL fixture(s)!";
		}
	}

	void CollisionContact::EndContact(b2Contact* contact)
	{
		// At the moment we don't care what happens when a collision ends.
		// This would be implemented later tho.
	}
}