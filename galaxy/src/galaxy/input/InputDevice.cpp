///
/// InputDevice.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "InputDevice.hpp"

namespace galaxy
{
	namespace input
	{
		InputDevice::InputDevice() noexcept
			: m_window {nullptr}
		{
		}

		InputDevice::~InputDevice() noexcept
		{
			m_window = nullptr;
		}

		void InputDevice::set_window(GLFWwindow* window) noexcept
		{
			m_window = window;
		}
	} // namespace input
} // namespace galaxy