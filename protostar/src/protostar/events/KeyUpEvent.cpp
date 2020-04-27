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
	KeyUpEvent::KeyUpEvent(const protostar::Keys keycode) noexcept
		:m_keycode(keycode)
	{
	}
}