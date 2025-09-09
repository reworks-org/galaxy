///
/// MouseReleased.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSERELEASED_HPP_
#define GALAXY_EVENTS_MOUSERELEASED_HPP_

#include "galaxy/events/HandleableEvent.hpp"
#include "galaxy/input/MouseButton.hpp"

namespace galaxy
{
	///
	/// Contains data relating to a mouse released event.
	///
	struct MouseReleased final : public HandleableEvent
	{
		///
		/// X coordinate, relative to window.
		///
		float m_xpos = 0.0f;

		///
		/// Y coordinate, relative to window.
		///
		float m_ypos = 0.0f;

		///
		/// 1 for single-click, 2 for double-click, etc.
		///
		int m_clicks = 0;

		///
		/// The mouse button index.
		///
		MouseButton m_button = MouseButton::UNKNOWN;
	};
} // namespace galaxy

#endif
