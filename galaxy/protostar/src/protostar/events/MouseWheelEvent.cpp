///
/// MouseWheelEvent.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseWheelEvent.hpp"

///
/// Core namespace.
///
namespace pr
{
	MouseWheelEvent::MouseWheelEvent()
	    : m_x_offset {0.0}, m_y_offset {0.0}
	{
	}

	MouseWheelEvent::MouseWheelEvent(const double x, const double y)
	    : m_x_offset {x}, m_y_offset {y}
	{
	}
} // namespace pr