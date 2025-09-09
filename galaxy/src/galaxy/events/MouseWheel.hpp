///
/// MouseWheel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEWHEEL_HPP_
#define GALAXY_EVENTS_MOUSEWHEEL_HPP_

#include "galaxy/events/HandleableEvent.hpp"
#include "galaxy/input/MouseButton.hpp"

namespace galaxy
{
	///
	/// Contains mouse wheel movement data.
	///
	struct MouseWheel final : public HandleableEvent
	{
		///
		/// The amount scrolled horizontally, positive to the right and negative to the left.
		///
		float m_amount_x;

		///
		/// The amount scrolled vertically, positive away from the user and negative toward the user.
		///
		float m_amount_y;

		///
		/// When FLIPPED the values in X and Y will be opposite.
		/// Multiply by -1 to change them back.
		///
		MouseWheelDirection m_direction;

		///
		/// X coordinate, relative to window.
		///
		float m_mouse_x;

		///
		/// Y coordinate, relative to window.
		///
		float m_mouse_y;

		///
		/// The amount scrolled horizontally, accumulated to whole scroll "ticks".
		///
		int m_total_x;

		///
		/// The amount scrolled vertically, accumulated to whole scroll "ticks".
		///
		int m_total_y;
	};
} // namespace galaxy

#endif
