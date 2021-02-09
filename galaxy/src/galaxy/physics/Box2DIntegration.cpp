///
/// Box2DIntegration.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/events/dispatcher/Dispatcher.hpp"
#include "galaxy/events/Collision.hpp"
#include "galaxy/events/FinishCollision.hpp"

#include "Box2DIntegration.hpp"

namespace galaxy
{
	namespace physics
	{
		GalaxyContactListener::GalaxyContactListener(events::Dispatcher* dispatcher)
		    : m_dispatcher {dispatcher}
		{
		}

		void GalaxyContactListener::BeginContact(b2Contact* contact)
		{
			m_dispatcher->trigger<events::Collision>(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody());
		}

		void GalaxyContactListener::EndContact(b2Contact* contact)
		{
			m_dispatcher->trigger<events::FinishCollision>(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody());
		}

		void GalaxyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
		{
			B2_NOT_USED(contact);
			B2_NOT_USED(oldManifold);
		}

		void GalaxyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
		{
			B2_NOT_USED(contact);
			B2_NOT_USED(impulse);
		}
	} // namespace physics
} // namespace galaxy