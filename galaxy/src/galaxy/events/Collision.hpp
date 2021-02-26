///
/// Collision.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_COLLISION_HPP_
#define GALAXY_EVENTS_COLLISION_HPP_

#include "galaxy/ecs/Entity.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Event triggered when two entities first touch.
		///
		struct Collision final
		{
			///
			/// Default constructor.
			///
			Collision() noexcept = default;

			///
			/// Argument constructor.
			///
			/// \param a First entity.
			/// \param b Second entity.
			///
			Collision(const ecs::Entity a, const ecs::Entity b) noexcept;

			///
			/// Default destructor.
			///
			~Collision() noexcept = default;

			///
			/// First entity.
			///
			ecs::Entity m_a;

			///
			/// Second entity.
			///
			ecs::Entity m_b;
		};
	} // namespace events
} // namespace galaxy

#endif