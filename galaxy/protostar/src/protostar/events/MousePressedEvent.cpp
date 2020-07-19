///
/// MousePressedEvent.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "MousePressedEvent.hpp"

///
/// Core namespace.
///
namespace pr
{
	MousePressedEvent::MousePressedEvent() noexcept
	    : m_x(0), m_y(0), m_button(0)
	{
	}

	MousePressedEvent::MousePressedEvent(const int x, const int y, const unsigned int button) noexcept
	    : m_x(x), m_y(y), m_button(button)
	{
	}
} // namespace pr