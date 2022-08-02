///
/// Keyboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include "Keyboard.hpp"

namespace galaxy
{
	namespace input
	{
		Keyboard::Keyboard() noexcept
			: InputDevice {}
			, m_text_input_enabled {false}
		{
		}

		void Keyboard::begin_text_input() noexcept
		{
			m_text_input_enabled = true;
		}

		void Keyboard::end_text_input() noexcept
		{
			m_text_input_enabled = false;
		}

		void Keyboard::enable_sticky_keys() const noexcept
		{
			glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
			glfwSetInputMode(m_window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
		}

		void Keyboard::disable_sticky_keys() const noexcept
		{
			glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_FALSE);
			glfwSetInputMode(m_window, GLFW_LOCK_KEY_MODS, GLFW_FALSE);
		}

		bool Keyboard::is_text_input_enabled() const noexcept
		{
			return m_text_input_enabled;
		}

		int Keyboard::get_scancode(const input::Keys key) const noexcept
		{
			return glfwGetKeyScancode(static_cast<int>(key));
		}

		std::string Keyboard::get_key_name(const input::Keys key) const noexcept
		{
			return glfwGetKeyName(static_cast<int>(key), 0);
		}

		std::string Keyboard::get_scancode_name(const int scancode) const noexcept
		{
			return glfwGetKeyName(GLFW_KEY_UNKNOWN, scancode);
		}
	} // namespace input
} // namespace galaxy