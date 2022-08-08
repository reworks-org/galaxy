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
			input::Keys m_keycode;

			///
			/// Modifier Key.
			///
			input::InputMods m_mod;

			///
			/// Platform scancode.
			///
			int m_scancode;

			///
			/// Has this event been handled?
			///
			bool m_handled = false;
		};
	} // namespace events
} // namespace galaxy

#endif