///
/// MouseReleasedEvent.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseReleasedEvent.hpp"

///
/// Core namespace.
///
namespace protostar
{
	MouseReleasedEvent::MouseReleasedEvent() noexcept
		:m_x(0), m_y(0), m_button(0)
	{
	}

	MouseReleasedEvent::MouseReleasedEvent(const int x, const int y, const unsigned int button) noexcept
		:m_x(x), m_y(y), m_button(button)
	{
	}
}