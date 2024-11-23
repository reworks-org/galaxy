///
/// Keyboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Keyboard.hpp"

namespace galaxy
{
	namespace input
	{
		namespace keyboard
		{
			bool is_key_pressed(const ray::KeyboardKey key) noexcept
			{
				return ::IsKeyPressed(static_cast<int>(key));
			}

			bool is_key_repeating(const ray::KeyboardKey key) noexcept
			{
				return ::IsKeyPressedRepeat(static_cast<int>(key));
			}

			bool is_key_released(const ray::KeyboardKey key) noexcept
			{
				return ::IsKeyReleased(static_cast<int>(key));
			}

			bool is_key_down(const ray::KeyboardKey key) noexcept
			{
				return ::IsKeyDown(static_cast<int>(key));
			}

			bool is_key_up(const ray::KeyboardKey key) noexcept
			{
				return ::IsKeyUp(static_cast<int>(key));
			}

			ray::KeyboardKey get_key_pressed() noexcept
			{
				return static_cast<ray::KeyboardKey>(::GetKeyPressed());
			}

			ray::KeyboardKey get_char_pressed() noexcept
			{
				return static_cast<ray::KeyboardKey>(::GetCharPressed());
			}
		} // namespace keyboard
	} // namespace input
} // namespace galaxy
