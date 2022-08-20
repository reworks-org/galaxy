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
	namespace events
	{
		///
		/// Contains data relating to a mouse moved event.
		///
		struct MouseMoved final
		{
			///
			/// Mouse x position.
			///
			double m_xpos = 0.0;

			///
			/// Mouse y position.
			///
			double m_ypos = 0.0;

			///
			/// Has this event been handled?
			///
			bool m_handled = false;
		};
	} // namespace events
} // namespace galaxy

#endif