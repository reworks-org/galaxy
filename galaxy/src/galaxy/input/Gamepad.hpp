///
/// Gamepad.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_GAMEPAD_HPP_
#define GALAXY_INPUT_GAMEPAD_HPP_

#include <string>

#include <raylib.hpp>

namespace galaxy
{
	namespace input
	{
		class Gamepad
		{
		public:
			///
			/// Index constructor.
			///
			/// \param gamepad The gamepad index.
			///
			Gamepad(const int gamepad) noexcept;

			///
			/// Destructor.
			///
			~Gamepad() noexcept;

			///
			/// Check if a gamepad is available.
			///
			/// \return True if the gamepad is available, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_available() const noexcept;

			///
			/// Get gamepad internal name id.
			///
			/// \return The name of the gamepad.
			///
			[[nodiscard]]
			std::string get_gamepad_name() noexcept;

			///
			/// Check if a gamepad button has been pressed once.
			///
			/// \param button The button index.
			///
			/// \return True if the gamepad button has been pressed once, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_pressed(const ray::GamepadButton button) const noexcept;

			///
			/// Check if a gamepad button is being pressed.
			///
			/// \param button The button index.
			///
			/// \return True if the gamepad button is being pressed, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_down(const ray::GamepadButton button) const noexcept;

			///
			/// Check if a gamepad button has been released once.
			///
			/// \param button The button index.
			///
			/// \return True if the gamepad button has been released once, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_released(const ray::GamepadButton button) const noexcept;

			///
			/// Check if a gamepad button is NOT being pressed.
			///
			/// \param button The button index.
			///
			/// \return True if the gamepad button is NOT being pressed, false otherwise.
			///
			[[nodiscard]]
			bool is_gamepad_button_up(const ray::GamepadButton button) const noexcept;

			///
			/// Get the last gamepad button pressed.
			///
			/// \return The index of the last gamepad button pressed.
			///
			[[nodiscard]]
			int get_gamepad_button_pressed() const noexcept;

			///
			/// Get gamepad axis count for a gamepad.
			///
			/// \return The number of axes for the gamepad.
			///
			[[nodiscard]]
			int get_gamepad_axis_count() const noexcept;

			///
			/// Get axis movement value for a gamepad axis.
			///
			/// \param axis The axis index.
			///
			/// \return The movement value for the specified axis.
			///
			[[nodiscard]]
			float get_gamepad_axis_movement(const ray::GamepadAxis axis) const noexcept;

			///
			/// Set internal gamepad mappings (SDL_GameControllerDB).
			///
			/// \param mappings The mappings string.
			///
			/// \return 0 on success, -1 on failure.
			///
			[[nodiscard]]
			int set_gamepad_mappings(const std::string& mappings) noexcept;

			///
			/// Set gamepad vibration for both motors (duration in seconds).
			///
			/// \param left_motor The intensity of the left motor vibration.
			/// \param right_motor The intensity of the right motor vibration.
			/// \param duration The duration of the vibration in seconds.
			///
			void set_gamepad_vibration(const float left_motor, const float right_motor, const float duration) const noexcept;

		private:
			///
			/// Constructor.
			///
			Gamepad() = delete;

		private:
			///
			/// The gamepad index.
			///
			int m_gamepad;
		};
	} // namespace input
} // namespace galaxy

#endif
