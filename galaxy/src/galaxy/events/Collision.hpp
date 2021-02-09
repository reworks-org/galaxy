///
/// Collision.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_COLLISION_HPP_
#define GALAXY_EVENTS_COLLISION_HPP_

#include "galaxy/physics/b2_body.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Event triggered when two bodies first touch.
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
			/// \param a First body.
			/// \param b Second body.
			///
			Collision(b2Body* a, b2Body* b) noexcept;

			///
			/// Default destructor.
			///
			~Collision() noexcept = default;

			///
			/// First body.
			///
			b2Body* m_a;

			///
			/// Second body.
			///
			b2Body* m_b;
		};
	} // namespace events
} // namespace galaxy

#endif