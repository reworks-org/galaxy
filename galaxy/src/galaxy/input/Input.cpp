///
/// Input.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include "Input.hpp"

namespace galaxy
{
	namespace input
	{
		bool Input::key_down(input::Keys key) noexcept
		{
			return glfwGetKey(s_window, static_cast<int>(key)) == GLFW_PRESS;
		}

		bool Input::mouse_button_down(input::Mouse::Buttons button) noexcept
		{
			return glfwGetMouseButton(s_window, static_cast<int>(button)) == GLFW_PRESS;
		}

		const glm::dvec2& Input::get_cursor_pos() noexcept
		{
			glfwGetCursorPos(s_window, &s_cursor_pos.x, &s_cursor_pos.y);
			return s_cursor_pos;
		}
	} // namespace input
} // namespace galaxy