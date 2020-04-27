///
/// KeyCharEvent.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyCharEvent.hpp"

///
/// Core namespace.
///
namespace protostar
{
	KeyCharEvent::KeyCharEvent() noexcept
		:m_unichar(0)
	{
	}

	KeyCharEvent::KeyCharEvent(const protostar::Keys keycode, const int unichar) noexcept
		:m_keycode(keycode), m_unichar(unichar)
	{
	}
}