///
/// KeyPress.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYPRESS_HPP_
#define GALAXY_EVENTS_KEYPRESS_HPP_

#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was pressed down.
		///
		struct KeyPress final
		{
			///
			/// Keycode for the key that was pressed.
			///
			input::Keys keycode = input::Keys::KEY_UNKNOWN;

			///
			/// Modifier Key.
			///
			input::InputMods mod = input::InputMods::UNKNOWN;

			///
			/// Platform scancode.
			///
			int scancode = 0;

			///
			/// Has this event been handled?
			///
			bool handled = false;

			///
			/// Was this a press (down) or release (up) event.
			///
			bool pressed = false;

			///
			/// Is key repeating?
			///
			bool repeat = false;
		};
	} // namespace events
} // namespace galaxy

#endif
