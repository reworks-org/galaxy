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
	///
	/// Contains the key that was pressed down.
	///
	struct KeyRepeat final : public HandleableEvent
	{
		///
		/// Keycode for the key that is repeating.
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
	};
} // namespace galaxy

#endif
