///
/// Frame.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIM_FRAME_HPP_
#define GALAXY_GRAPHICS_ANIM_FRAME_HPP_

#include "galaxy/math/Rect.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Single frame of an animation.
		///
		struct Frame final
		{
			///
			/// Texture bounds.
			///
			math::fRect m_bounds;

			///
			/// Time to spend on this frame in milliseconds.
			///
			double m_duration = 200;
		};
	} // namespace graphics
} // namespace galaxy

#endif
