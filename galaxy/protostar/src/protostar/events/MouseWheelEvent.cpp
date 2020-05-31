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
namespace protostar
{
	MouseWheelEvent::MouseWheelEvent() noexcept
		:m_xOffset(0.0), m_yOffset(0.0)
	{
	}

	MouseWheelEvent::MouseWheelEvent(const float x, const float y) noexcept
		:m_xOffset(x), m_yOffset(y)
	{
	}
}