///
/// KeyDown.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYDOWN_HPP_
#define GALAXY_EVENTS_KEYDOWN_HPP_

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was pressed down.
		///
		struct KeyDown final
		{
			///
			/// Keycode for the key that was pressed.
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
		};
	} // namespace events
} // namespace galaxy

#endif