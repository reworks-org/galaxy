///
/// MousePressed.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MousePressed.hpp"

namespace galaxy
{
	namespace events
	{
		MousePressed::MousePressed() noexcept
		    : m_x {0}, m_y {0}, m_button {0}
		{
		}

		MousePressed::MousePressed(const float x, const float y, const input::MouseButtons button) noexcept
		    : m_x {x}, m_y {y}, m_button {button}
		{
		}
	} // namespace events
} // namespace galaxy