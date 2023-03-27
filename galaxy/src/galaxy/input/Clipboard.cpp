///
/// Clipboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include "Clipboard.hpp"

namespace galaxy
{
	namespace input
	{
		Clipboard::Clipboard()
			: InputDevice {}
		{
		}

		void Clipboard::set(const char* contents) const
		{
			glfwSetClipboardString(m_window, contents);
		}

		std::string Clipboard::get() const
		{
			return glfwGetClipboardString(m_window);
		}
	} // namespace input
} // namespace galaxy