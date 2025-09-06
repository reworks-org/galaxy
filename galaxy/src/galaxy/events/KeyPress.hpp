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
	///
	/// Contains the key that was pressed down.
	///
	struct KeyPress final : public HandleableEvent
	{
		///
		/// Keycode for the key that was pressed.
		///
		input::Keys m_keycode = input::Keys::KEY_UNKNOWN;

		///
		/// Modifier Key.
		///
		input::InputMods m_mod = input::InputMods::UNKNOWN;

		///
		/// Platform scancode.
		///
		int m_scancode = 0;

		///
		/// Was this a press (down) or release (up) event.
		///
		bool m_pressed = false;

		///
		/// Is key repeating?
		///
		bool m_repeat = false;
	};
} // namespace galaxy

#endif
