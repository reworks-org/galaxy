///
/// MouseButtons.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSEBUTTONS_HPP_
#define GALAXY_INPUT_MOUSEBUTTONS_HPP_

#include <GLFW/glfw3.h>

namespace galaxy
{
	namespace input
	{
		///
		/// \brief Enum class representing mouse buttons.
		///
		/// Values used are based off of GLFW.
		///
		enum class MouseButtons : int
		{
			UNKNOWN    = -1,
			BTN_1      = GLFW_MOUSE_BUTTON_1,
			BTN_2      = GLFW_MOUSE_BUTTON_2,
			BTN_3      = GLFW_MOUSE_BUTTON_3,
			BTN_4      = GLFW_MOUSE_BUTTON_4,
			BTN_5      = GLFW_MOUSE_BUTTON_5,
			BTN_6      = GLFW_MOUSE_BUTTON_6,
			BTN_7      = GLFW_MOUSE_BUTTON_7,
			BTN_8      = GLFW_MOUSE_BUTTON_8,
			BTN_LAST   = GLFW_MOUSE_BUTTON_8,
			BTN_LEFT   = GLFW_MOUSE_BUTTON_1,
			BTN_RIGHT  = GLFW_MOUSE_BUTTON_2,
			BTN_MIDDLE = GLFW_MOUSE_BUTTON_3
		};

		///
		/// Converts an int to a mouse button.
		///
		/// \param mouse Integer mouse button.
		///
		/// \return Enum mouse button.
		///
		[[nodiscard]] inline constexpr input::MouseButtons int_to_mouse(const int mouse)
		{
			return static_cast<input::MouseButtons>(mouse);
		}

		///
		/// Converts a mouse button to an int.
		///
		/// \param mouse Enum mouse button.
		///
		/// \return Integer mouse button.
		///
		[[nodiscard]] inline constexpr int mouse_to_int(const input::MouseButtons mouse)
		{
			return static_cast<int>(mouse);
		}
	} // namespace input
} // namespace galaxy

#endif
