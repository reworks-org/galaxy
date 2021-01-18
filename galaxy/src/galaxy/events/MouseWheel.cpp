///
/// MouseWheel.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseWheel.hpp"

namespace galaxy
{
	namespace events
	{
		MouseWheel::MouseWheel() noexcept
		    : m_x_offset {0.0}, m_y_offset {0.0}
		{
		}

		MouseWheel::MouseWheel(const double x, const double y) noexcept
		    : m_x_offset {x}, m_y_offset {y}
		{
		}
	} // namespace events
} // namespace galaxy