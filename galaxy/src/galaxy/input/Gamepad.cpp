///
/// Gamepad.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Gamepad.hpp"

namespace galaxy
{
	namespace input
	{
		namespace gamepad
		{
			bool is_gamepad_available(const int gamepad)
			{
				return IsGamepadAvailable(gamepad);
			}

			std::string get_gamepad_name(const int gamepad)
			{
				return {GetGamepadName(gamepad)};
			}

			bool is_gamepad_button_pressed(const int gamepad, const GamepadButton button)
			{
				return IsGamepadButtonPressed(gamepad, button);
			}

			bool is_gamepad_button_down(const int gamepad, const GamepadButton button)
			{
				return IsGamepadButtonDown(gamepad, button);
			}

			bool is_gamepad_button_released(const int gamepad, const GamepadButton button)
			{
				return IsGamepadButtonReleased(gamepad, button);
			}

			bool is_gamepad_button_up(const int gamepad, const GamepadButton button)
			{
				return IsGamepadButtonUp(gamepad, button);
			}

			int get_gamepad_button_pressed()
			{
				return GetGamepadButtonPressed();
			}

			int get_gamepad_axis_count(const int gamepad)
			{
				return GetGamepadAxisCount(gamepad);
			}

			float get_gamepad_axis_movement(const int gamepad, const GamepadAxis axis)
			{
				return GetGamepadAxisMovement(gamepad, axis);
			}

			int set_gamepad_mappings(const std::string& mappings)
			{
				return SetGamepadMappings(mappings.c_str());
			}

			void set_gamepad_vibration(const int gamepad, const float left_motor, const float right_motor, const float duration)
			{
				SetGamepadVibration(gamepad, left_motor, right_motor, duration);
			}
		} // namespace gamepad
	} // namespace input
} // namespace galaxy
