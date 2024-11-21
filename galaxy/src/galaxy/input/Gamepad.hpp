///
/// Gamepad.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_GAMEPAD_HPP_
#define GALAXY_INPUT_GAMEPAD_HPP_

#include <string>

#include <raylib.h>

namespace galaxy
{
	namespace input
	{
		namespace gamepad
		{
			///
			/// Check if a gamepad is available.
			///
			/// \param gamepad The gamepad index.
			///
			/// \return True if the gamepad is available, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_available(const int gamepad);

			///
			/// Get gamepad internal name id.
			///
			/// \param gamepad The gamepad index.
			///
			/// \return The name of the gamepad.
			///
			[[nodiscard]]
			std::string get_gamepad_name(const int gamepad);

			///
			/// Check if a gamepad button has been pressed once.
			///
			/// \param gamepad The gamepad index.
			/// \param button The button index.
			///
			/// \return True if the gamepad button has been pressed once, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_pressed(const int gamepad, const GamepadButton button);

			///
			/// Check if a gamepad button is being pressed.
			///
			/// \param gamepad The gamepad index.
			/// \param button The button index.
			///
			/// \return True if the gamepad button is being pressed, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_down(const int gamepad, const GamepadButton button);

			///
			/// Check if a gamepad button has been released once.
			///
			/// \param gamepad The gamepad index.
			/// \param button The button index.
			///
			/// \return True if the gamepad button has been released once, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_released(const int gamepad, const GamepadButton button);

			///
			/// Check if a gamepad button is NOT being pressed.
			///
			/// \param gamepad The gamepad index.
			/// \param button The button index.
			///
			/// \return True if the gamepad button is NOT being pressed, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_up(const int gamepad, const GamepadButton button);

			///
			/// Get the last gamepad button pressed.
			///
			/// \return The index of the last gamepad button pressed.
			///
			[[nodiscard]]
			int get_gamepad_button_pressed();

			///
			/// Get gamepad axis count for a gamepad.
			///
			/// \param gamepad The gamepad index.
			///
			/// \return The number of axes for the gamepad.
			///
			[[nodiscard]]
			int get_gamepad_axis_count(const int gamepad);

			///
			/// Get axis movement value for a gamepad axis.
			///
			/// \param gamepad The gamepad index.
			/// \param axis The axis index.
			///
			/// \return The movement value for the specified axis.
			///
			[[nodiscard]]
			float get_gamepad_axis_movement(const int gamepad, const GamepadAxis axis);

			///
			/// Set internal gamepad mappings (SDL_GameControllerDB).
			///
			/// \param mappings The mappings string.
			///
			/// \return 0 on success, -1 on failure.
			///
			[[nodiscard]]
			int set_gamepad_mappings(const std::string& mappings);

			///
			/// Set gamepad vibration for both motors (duration in seconds).
			///
			/// \param gamepad The gamepad index.
			/// \param left_motor The intensity of the left motor vibration.
			/// \param right_motor The intensity of the right motor vibration.
			/// \param duration The duration of the vibration in seconds.
			///
			void set_gamepad_vibration(const int gamepad, const float left_motor, const float right_motor, const float duration);

		} // namespace gamepad
	} // namespace input
} // namespace galaxy

#endif
