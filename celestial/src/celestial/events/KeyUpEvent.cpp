///
/// KeyUpEvent.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyUpEvent.hpp"

///
/// Core namespace.
///
namespace celestial
{
	KeyUpEvent::KeyUpEvent() noexcept
		:m_keycode(0)
	{
	}

	KeyUpEvent::KeyUpEvent(const int keycode) noexcept
		:m_keycode(keycode)
	{
	}
}