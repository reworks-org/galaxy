///
/// Input.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL3/SDL_keyboard.h>

#include "Input.hpp"

namespace galaxy
{
	namespace input
	{
		bool is_key_mod_down(const KeyMods modifier) noexcept
		{
			const auto mod = static_cast<SDL_Keymod>(modifier);
			return SDL_GetModState() & mod;
		}

		bool is_key_down(const Keys key) noexcept
		{
			const auto keys = SDL_GetKeyboardState(nullptr);
			return keys[static_cast<SDL_Keycode>(key)];
		}

		bool is_mouse_down(const MouseButton btn) noexcept
		{
			const auto buttons = SDL_GetMouseState(nullptr, nullptr);
			return buttons & SDL_BUTTON_MASK(static_cast<SDL_MouseButtonFlags>(btn));
		}

		glm::vec2 get_cursor_pos() noexcept
		{
			glm::vec2 vec2;
			SDL_GetMouseState(&vec2.x, &vec2.y);

			return vec2;
		}
	} // namespace input
} // namespace galaxy
