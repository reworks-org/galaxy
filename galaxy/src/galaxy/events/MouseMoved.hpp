///
/// MouseMoved.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEMOVED_HPP_
#define GALAXY_EVENTS_MOUSEMOVED_HPP_

namespace galaxy
{
	///
	/// Contains data relating to a mouse moved event.
	///
	struct MouseMoved final : public HandleableEvent
	{
		///
		/// Mouse x position.
		///
		double m_xpos = 0.0;

		///
		/// Mouse y position.
		///
		double m_ypos = 0.0;
	};
} // namespace galaxy

#endif
