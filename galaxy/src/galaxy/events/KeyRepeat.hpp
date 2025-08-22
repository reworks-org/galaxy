///
/// KeyRepeat.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYREPEAT_HPP_
#define GALAXY_EVENTS_KEYREPEAT_HPP_

#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was pressed down.
		///
		struct KeyRepeat final
		{
			///
			/// Keycode for the key that is repeating.
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
		};
	} // namespace events
} // namespace galaxy

#endif
