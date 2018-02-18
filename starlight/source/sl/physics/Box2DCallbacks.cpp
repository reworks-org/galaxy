///
/// Box2DCallbacks.cpp
/// starlight
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "entt/entity/registry.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/events/EventManager.hpp"
#include "sl/events/CollisionEvent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"

#include "Box2DCallbacks.hpp"

namespace sl
{
	void CollisionContact::BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		entt::Entity* a = static_cast<entt::Entity*>(fixtureA->GetUserData());
		entt::Entity* b = static_cast<entt::Entity*>(fixtureB->GetUserData());
		
		Locator::m_eventManager->emitEvent(EventTypes::COLLISION_EVENT, (intptr_t)(new CollisionEvent(*a, *b, fixtureA->GetFilterData().categoryBits, fixtureB->GetFilterData().categoryBits)), NULL, NULL, NULL, [](ALLEGRO_USER_EVENT* uev)
		{  
			delete (CollisionEvent*)uev->data1;
		});
	}

	void CollisionContact::EndContact(b2Contact* contact)
	{
	}
}