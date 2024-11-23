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
		Gamepad::Gamepad(const int gamepad) noexcept
			: m_gamepad(gamepad)
		{
		}

		Gamepad::~Gamepad() noexcept
		{
		}

		bool Gamepad::is_gamepad_available() const noexcept
		{
			return ::IsGamepadAvailable(m_gamepad);
		}

		std::string Gamepad::get_gamepad_name() noexcept
		{
			return {::GetGamepadName(m_gamepad)};
		}

		bool Gamepad::is_gamepad_button_pressed(const ray::GamepadButton button) const noexcept
		{
			return ::IsGamepadButtonPressed(m_gamepad, button);
		}

		bool Gamepad::is_gamepad_button_down(const ray::GamepadButton button) const noexcept
		{
			return ::IsGamepadButtonDown(m_gamepad, button);
		}

		bool Gamepad::is_gamepad_button_released(const ray::GamepadButton button) const noexcept
		{
			return ::IsGamepadButtonReleased(m_gamepad, button);
		}

		bool Gamepad::is_gamepad_button_up(const ray::GamepadButton button) const noexcept
		{
			return ::IsGamepadButtonUp(m_gamepad, button);
		}

		int Gamepad::get_gamepad_button_pressed() const noexcept
		{
			return ::GetGamepadButtonPressed();
		}

		int Gamepad::get_gamepad_axis_count() const noexcept
		{
			return ::GetGamepadAxisCount(m_gamepad);
		}

		float Gamepad::get_gamepad_axis_movement(const ray::GamepadAxis axis) const noexcept
		{
			return ::GetGamepadAxisMovement(m_gamepad, axis);
		}

		int Gamepad::set_gamepad_mappings(const std::string& mappings) noexcept
		{
			return ::SetGamepadMappings(mappings.c_str());
		}

		void Gamepad::set_gamepad_vibration(const float left_motor, const float right_motor, const float duration) const noexcept
		{
			::SetGamepadVibration(m_gamepad, left_motor, right_motor, duration);
		}
	} // namespace input
} // namespace galaxy
