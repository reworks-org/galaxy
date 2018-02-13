///
/// Box2DCallbacks.cpp
/// starlight
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "entt/entity/registry.hpp"
#include "sl/events/EventManager.hpp"
#include "sl/events/CollisionEvent.hpp"

#include "Box2DCallbacks.hpp"

namespace sl
{
	void CollisionContact::BeginContact(b2Contact* contact)
	{
		entt::Entity* a = static_cast<entt::Entity*>(contact->GetFixtureA()->GetUserData());
		entt::Entity* b = static_cast<entt::Entity*>(contact->GetFixtureB()->GetUserData());

		EventManager::inst()->emitEvent(EventTypes::COLLISION_EVENT, (int*)CollisionEvent {*a, *b});
	}

	void CollisionContact::EndContact(b2Contact* contact)
	{
	}
}