///
/// KeyUp.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYUP_HPP_
#define GALAXY_EVENTS_KEYUP_HPP_

#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was released.
		///
		struct KeyUp final
		{
			///
			/// Keycode for the key that was released.
			///
			input::Keys keycode = input::Keys::UNKNOWN;

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