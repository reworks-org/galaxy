///
/// Keyboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_KEYBOARD_HPP_
#define GALAXY_INPUT_KEYBOARD_HPP_

#include <raylib.hpp>

namespace galaxy
{
	namespace input
	{
		///
		/// Keyboard device related functionality.
		///
		namespace keyboard
		{
			///
			/// Check if a key was pressed once.
			///
			/// \param key Keyboard key to check.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_key_pressed(const ray::KeyboardKey key) noexcept;

			///
			/// Check if a key is being pressed repeatedly.
			///
			/// \param key Keyboard key to check.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_key_repeating(const ray::KeyboardKey key) noexcept;

			///
			/// Check if a key that was pressed is now released.
			///
			/// \param key Keyboard key to check.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_key_released(const ray::KeyboardKey key) noexcept;

			///
			/// Check if a key is being held down.
			///
			/// \param key Keyboard key to check.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_key_down(const ray::KeyboardKey key) noexcept;

			///
			/// Check if a key is NOT being pressed.
			///
			/// \param key Keyboard key to check.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_key_up(const ray::KeyboardKey key) noexcept;

			///
			/// \brief Get key pressed (keycode).
			///
			/// Call it multiple times for keys queued, returns 0 when the queue is empty.
			///
			/// \return Keyboard key enum.
			///
			[[nodiscard]]
			ray::KeyboardKey get_key_pressed() noexcept;

			///
			/// \brief Get char pressed (unicode).
			///
			/// Call it multiple times for chars queued, returns 0 when the queue is empty.
			///
			/// \return Keyboard key enum.
			///
			[[nodiscard]]
			ray::KeyboardKey get_char_pressed() noexcept;
		} // namespace keyboard
	} // namespace input
} // namespace galaxy

#endif
