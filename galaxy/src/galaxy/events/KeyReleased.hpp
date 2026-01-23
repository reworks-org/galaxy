///
/// KeyReleased.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYRELEASED_HPP_
#define GALAXY_EVENTS_KEYRELEASED_HPP_

#include "galaxy/events/Event.hpp"
#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	///
	/// Contains data on key that was released.
	///
	struct KeyReleased final : public Event
	{
		///
		/// Constructor.
		///
		/// \param scancode SDL physical key code.
		/// \param key SDL virtual key code.
		/// \param mod Current key modifiers.
		/// \param raw The platform dependent scancode for this event.
		/// \param repeat True if this is a key repeat.
		///
		KeyReleased(const SDL_Scancode scancode, const Keys key, const KeyMods mod, const unsigned short raw, const bool repeat) noexcept;

		///
		/// SDL physical key code.
		///
		SDL_Scancode m_scancode = SDL_SCANCODE_UNKNOWN;

		///
		/// SDL virtual key code.
		///
		Keys m_key = Keys::KEY_UNKNOWN;

		///
		/// Current key modifiers.
		///
		KeyMods m_mod = KeyMods::MOD_NONE;

		///
		/// The platform dependent scancode for this event.
		///
		unsigned short m_raw = 0;

		///
		/// True if this is a key repeat.
		///
		bool m_repeat = false;
	};
} // namespace galaxy

#endif
