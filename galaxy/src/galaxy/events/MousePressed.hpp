///
/// MousePressed.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEPRESSED_HPP_
#define GALAXY_EVENTS_MOUSEPRESSED_HPP_

#include "galaxy/events/HandleableEvent.hpp"
#include "galaxy/input/MouseButton.hpp"

namespace galaxy
{
	///
	/// Contains data relating to a mouse pressed event.
	///
	struct MousePressed final : public HandleableEvent
	{
		///
		/// X coordinate, relative to window.
		///
		float m_xpos;

		///
		/// Y coordinate, relative to window.
		///
		float m_ypos;

		///
		/// 1 for single-click, 2 for double-click, etc.
		///
		int m_clicks;

		///
		/// The mouse button index.
		///
		MouseButton m_button;
	};
} // namespace galaxy

#endif
