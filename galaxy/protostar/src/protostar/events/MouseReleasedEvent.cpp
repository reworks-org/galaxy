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
namespace pr
{
	MouseReleasedEvent::MouseReleasedEvent()
	    : m_x {0}, m_y {0}, m_button {0}
	{
	}

	MouseReleasedEvent::MouseReleasedEvent(const float x, const float y, const int button)
	    : m_x {x}, m_y {y}, m_button {button}
	{
	}
} // namespace pr