///
/// WindowResized.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_WINDOWRESIZED_HPP_
#define GALAXY_EVENTS_WINDOWRESIZED_HPP_

#include "galaxy/events/Event.hpp"

namespace galaxy
{
	///
	/// New width and height of window being resized.
	///
	struct WindowResized final : public Event
	{
		///
		/// Constructor.
		///
		/// \param width New width of window.
		/// \param height New height of window.
		///
		WindowResized(const int width, const int height) noexcept;

		///
		/// New width of window.
		///
		int m_width = 0;

		///
		/// New height of window.
		///
		int m_height = 0;
	};
} // namespace galaxy

#endif
