///
/// MouseMoved.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEMOVED_HPP_
#define GALAXY_EVENTS_MOUSEMOVED_HPP_

#include "galaxy/events/Event.hpp"

namespace galaxy
{
	///
	/// Contains data relating to a mouse moved event.
	///
	struct MouseMoved final : public Event
	{
		///
		/// Constructor.
		///
		/// \param xpos X coordinate, relative to window.
		/// \param ypos Y coordinate, relative to window.
		/// \param xrel The relative motion in the X direction.
		/// \param yrel The relative motion in the Y direction.
		///
		MouseMoved(const float xpos, const float ypos, const float xrel, const float yrel) noexcept;

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
