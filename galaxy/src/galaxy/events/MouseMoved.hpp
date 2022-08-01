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
			double m_xpos;

			///
			/// Mouse y position.
			///
			double m_ypos;
		};
	} // namespace events
} // namespace galaxy

#endif