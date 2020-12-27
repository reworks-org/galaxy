///
/// MouseReleased.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseReleased.hpp"

namespace galaxy
{
	namespace events
	{
		MouseReleased::MouseReleased()
		    : m_x {0}, m_y {0}, m_button {0}
		{
		}

		MouseReleased::MouseReleased(const float x, const float y, const input::MouseButton button)
		    : m_x {x}, m_y {y}, m_button {button}
		{
		}
	} // namespace events
} // namespace galaxy