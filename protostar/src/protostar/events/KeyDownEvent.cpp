///
/// KeyDownEvent.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyDownEvent.hpp"

///
/// Core namespace.
///
namespace protostar
{
	KeyDownEvent::KeyDownEvent(const protostar::Keys keycode) noexcept
		:m_keycode(keycode)
	{
	}
}