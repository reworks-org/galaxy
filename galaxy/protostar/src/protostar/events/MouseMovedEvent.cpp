///
/// MouseMovedEvent.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseMovedEvent.hpp"

///
/// Core namespace.
///
namespace protostar
{
	MouseMovedEvent::MouseMovedEvent() noexcept
		:m_x(0), m_y(0), m_dx(0), m_dy(0), m_pressure(0.0f)
	{
	}

	MouseMovedEvent::MouseMovedEvent(const int x, const int y, const int dx, const int dy, const float pressure) noexcept
		:m_x(x), m_y(y), m_dx(dx), m_dy(dy), m_pressure(pressure)
	{
	}
}