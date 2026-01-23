///
/// MouseWheel.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseWheel.hpp"

namespace galaxy
{
	MouseWheel::MouseWheel(const float ax, const float ay, const SDL_MouseWheelDirection dir, const float mx, const float my, const int tx, const int ty) noexcept
		: m_amount_x {ax}
		, m_amount_y {ay}
		, m_direction {dir}
		, m_mouse_x {mx}
		, m_mouse_y {my}
		, m_total_x {tx}
		, m_total_y {ty}
	{
	}
} // namespace galaxy
