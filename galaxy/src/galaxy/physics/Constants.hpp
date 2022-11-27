///
/// Constants.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_CONSTANTS_HPP_
#define GALAXY_PHYSICS_CONSTANTS_HPP_

#include <box2d/b2_math.h>

namespace galaxy
{
	namespace physics
	{
		///
		/// Holds window related settings.
		///
		struct Constants final
		{
			///
			/// Gravity parameters for box2d.
			///
			static inline b2Vec2 gravity = {0, 0};

			///
			/// Box2D world velocity iterations.
			///
			static inline int velocity_iterations = 0;

			///
			/// Box2d world position iterations.
			///
			static inline int position_iterations = 0;

			///
			/// Scale for converting from box2d units to opengl pixels.
			///
			static inline float pixels_per_meter = 0.0f;
		};
	} // namespace physics
} // namespace galaxy

#endif