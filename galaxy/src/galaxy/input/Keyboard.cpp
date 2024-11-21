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
			bool is_key_pressed(const KeyboardKey key)
			{
				return IsKeyPressed(static_cast<int>(key));
			}

			bool is_key_repeating(const KeyboardKey key)
			{
				return IsKeyPressedRepeat(static_cast<int>(key));
			}

			bool is_key_released(const KeyboardKey key)
			{
				return IsKeyReleased(static_cast<int>(key));
			}

			bool is_key_down(const KeyboardKey key)
			{
				return IsKeyDown(static_cast<int>(key));
			}

			bool is_key_up(const KeyboardKey key)
			{
				return IsKeyUp(static_cast<int>(key));
			}

			KeyboardKey get_key_pressed()
			{
				return static_cast<KeyboardKey>(GetKeyPressed());
			}

			KeyboardKey get_char_pressed()
			{
				return static_cast<KeyboardKey>(GetCharPressed());
			}

		} // namespace keyboard
	} // namespace input
} // namespace galaxy

