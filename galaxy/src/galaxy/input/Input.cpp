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
		GLFWwindow* Input::s_window     = nullptr;
		glm::dvec2  Input::s_cursor_pos = {0.0, 0.0};

		bool Input::key_down(input::Keys key)
		{
			return glfwGetKey(s_window, static_cast<int>(key)) == GLFW_PRESS;
		}

		bool Input::mouse_button_down(input::MouseButtons button)
		{
			return glfwGetMouseButton(s_window, static_cast<int>(button)) == GLFW_PRESS;
		}

		const glm::dvec2& Input::get_cursor_pos()
		{
			glfwGetCursorPos(s_window, &s_cursor_pos.x, &s_cursor_pos.y);
			return s_cursor_pos;
		}
	} // namespace input
} // namespace galaxy
