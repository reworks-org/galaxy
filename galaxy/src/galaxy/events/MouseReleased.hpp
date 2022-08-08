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
			double m_xpos;

			///
			/// Mouse y position.
			///
			double m_ypos;

			///
			/// Mouse button released.
			///
			input::MouseButtons m_button;

			///
			/// Modifier Key.
			///
			input::InputMods m_mod;

			///
			/// Has this event been handled?
			///
			bool m_handled = false;
		};
	} // namespace events
} // namespace galaxy

#endif