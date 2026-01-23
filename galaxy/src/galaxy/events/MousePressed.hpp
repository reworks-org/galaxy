///
/// MousePressed.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEPRESSED_HPP_
#define GALAXY_EVENTS_MOUSEPRESSED_HPP_

#include "galaxy/events/Event.hpp"
#include "galaxy/input/MouseButton.hpp"

namespace galaxy
{
	///
	/// Contains data relating to a mouse pressed event.
	///
	struct MousePressed final : public Event
	{
		///
		/// Constructor.
		///
		/// \param xpos X coordinate, relative to window.
		/// \param ypos Y coordinate, relative to window.
		/// \param clicks 1 for single-click, 2 for double-click, etc.
		/// \param button The mouse button index.
		///
		MousePressed(const float xpos, const float ypos, const int clicks, const MouseButton button) noexcept;

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
