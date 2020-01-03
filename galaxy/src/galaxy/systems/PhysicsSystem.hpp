///
/// PhysicsSystem.hpp
/// galaxy
///
/// Created by reworks on 08/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICSSYSTEM_HPP_
#define GALAXY_PHYSICSSYSTEM_HPP_

#include <map>

#include "galaxy/types/System.hpp"
#include "galaxy/events/CollisionEvent.hpp"
#include "galaxy/libs/sol2/forward.hpp"

namespace galaxy
{
	///
	/// Process's entities with a PhysicsComponent.
	///
	class PhysicsSystem final : public System
	{
	public:
		///
		/// Construct the system.
		///
		/// \param functionScript Script containing physics collision functions.
		/// \param ups Updates per second for physics system.
		/// \param vi Box2D velocity iterations.
		/// \param pi Box2D position iterations.
		///
		PhysicsSystem(const std::string& functionScript, float ups, int vi, int pi);

		///
		/// Destructor.
		///
		~PhysicsSystem() noexcept override = default;

		///
		/// Function to connect to the event dispatcher.
		///
		/// \param ce PhysicsSystem will recieve collision events.
		///
		void receive(const CollisionEvent& ce);

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		PhysicsSystem() = delete;

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		/// \param dt Delta Time from update loop.
		/// \param registry Default entity registry.
		///
		void update(const double dt, entt::DefaultRegistry& registry) override;

	private:
		///
		/// Updates per second physics system should operate at.
		///
		float m_ups;

		///
		/// Step of physics system.
		///
		float m_step;

		///
		/// Number of iterations of velocity per update.
		///
		int m_velocityIterations;

		///
		/// Number of iterations of position per update.
		///
		int m_positionIterations;

		///
		/// Collision map function between two entities, using lua to define the functions to call.
		///
		std::map<std::pair<std::uint16_t, std::uint16_t>, sol::function> m_collisions;
	};
}

#endif