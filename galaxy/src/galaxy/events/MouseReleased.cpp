///
/// MouseReleased.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseReleased.hpp"

namespace galaxy
{
	MouseReleased::MouseReleased(const float xpos, const float ypos, const int clicks, const MouseButton button) noexcept
		: m_xpos {xpos}
		, m_ypos {ypos}
		, m_clicks {clicks}
		, m_button {button}
	{
	}
} // namespace galaxy
