///
/// KeyUpEvent.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyUpEvent.hpp"

///
/// Core namespace.
///
namespace protostar
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