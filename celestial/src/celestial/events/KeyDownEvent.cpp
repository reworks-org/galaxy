///
/// KeyDownEvent.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyDownEvent.hpp"

///
/// Core namespace.
///
namespace celestial
{
	KeyDownEvent::KeyDownEvent() noexcept
		:m_keycode(0)
	{
	}

	KeyDownEvent::KeyDownEvent(const int keycode) noexcept
		:m_keycode(keycode)
	{
	}
}