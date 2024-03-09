///
/// Mouse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include "Mouse.hpp"

namespace galaxy
{
	namespace input
	{
		Mouse::Mouse()
			: InputDevice {}
		{
		}

		Mouse::~Mouse()
		{
		}

		void Mouse::enable_sticky_mouse() const
		{
			glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
		}

		void Mouse::disable_sticky_mouse() const
		{
			glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
		}

		glm::dvec2 Mouse::get_pos()
		{
			glm::dvec2 out;
			glfwGetCursorPos(m_window, &out.x, &out.y);

			return out;
		}
	} // namespace input
} // namespace galaxy
