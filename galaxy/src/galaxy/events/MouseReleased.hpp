///
/// MouseReleased.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSERELEASED_HPP_
#define GALAXY_EVENTS_MOUSERELEASED_HPP_

#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/MouseButtons.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains data relating to a mouse released event.
		///
		struct MouseReleased final
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
			/// Mouse button released.
			///
			input::MouseButtons m_button = input::MouseButtons::UNKNOWN;

			///
			/// Modifier Key.
			///
			input::InputMods m_mod = input::InputMods::UNKNOWN;

			///
			/// Has this event been handled?
			///
			bool m_handled = false;
		};
	} // namespace events
} // namespace galaxy

#endif