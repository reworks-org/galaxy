///
/// MouseMoved.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEMOVED_HPP_
#define GALAXY_EVENTS_MOUSEMOVED_HPP_

#include "galaxy/events/HandleableEvent.hpp"

namespace galaxy
{
	///
	/// Contains data relating to a mouse moved event.
	///
	struct MouseMoved final : public HandleableEvent
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
		/// The relative motion in the X direction.
		///
		float m_xrel = 0.0f;

		///
		/// The relative motion in the Y direction.
		///
		float m_yrel = 0.0f;
	};
} // namespace galaxy

#endif
