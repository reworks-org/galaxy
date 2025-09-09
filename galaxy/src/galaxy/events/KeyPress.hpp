///
/// KeyPress.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYPRESS_HPP_
#define GALAXY_EVENTS_KEYPRESS_HPP_

#include <SDL3/SDL_keyboard.h>

#include "galaxy/events/HandleableEvent.hpp"

namespace galaxy
{
	///
	/// Contains the key that was pressed down.
	///
	struct KeyPress final : public HandleableEvent
	{
		///
		/// SDL physical key code.
		///
		SDL_Scancode m_scancode;

		///
		/// SDL virtual key code.
		///
		SDL_Keycode m_key;

		///
		/// Current key modifiers.
		///
		SDL_Keymod m_mod;

		///
		/// The platform dependent scancode for this event.
		///
		Uint16 m_raw;

		///
		/// True if this is a key repeat.
		///
		bool m_repeat;
	};
} // namespace galaxy

#endif
