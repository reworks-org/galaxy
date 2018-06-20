///
/// PhysicsSystem.hpp
/// starlight
///
/// Created by reworks on 08/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PHYSICSSYSTEM_HPP_
#define STARLIGHT_PHYSICSSYSTEM_HPP_

#include <map>

#include "sl/types/System.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
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
		~PhysicsSystem() override = default;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		PhysicsSystem() = delete;
		
		///
		/// Lets systems recieve events.
		///
		/// \param event ALLEGRO_EVENT passed by application class.
		///
		void event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry) override;

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		void update(const double dt, entt::DefaultRegistry& registry) override;

	private:
		///
		/// Updates per second physics system should operate at.
		///
		float m_ups;

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