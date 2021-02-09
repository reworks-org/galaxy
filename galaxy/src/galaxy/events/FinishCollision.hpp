///
/// FinishCollision.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_FINISHCOLLISION_HPP_
#define GALAXY_EVENTS_FINISHCOLLISION_HPP_

#include "galaxy/physics/b2_body.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Called when two bodies begin to seperate.
		///
		struct FinishCollision final
		{
			///
			/// Default constructor.
			///
			FinishCollision() noexcept = default;

			///
			/// Argument constructor.
			///
			/// \param a First body.
			/// \param b Second body.
			///
			FinishCollision(b2Body* a, b2Body* b) noexcept;

			///
			/// Default destructor.
			///
			~FinishCollision() noexcept = default;

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