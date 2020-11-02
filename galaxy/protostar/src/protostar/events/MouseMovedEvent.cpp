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
namespace pr
{
	MouseMovedEvent::MouseMovedEvent() noexcept
	    : m_x {0.0}, m_y {0.0}
	{
	}

	MouseMovedEvent::MouseMovedEvent(const double x, const double y) noexcept
	    : m_x {x}, m_y {y}
	{
	}
} // namespace pr