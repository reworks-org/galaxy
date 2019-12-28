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
		:m_keycode(0), m_unichar(0)
	{
	}

	KeyCharEvent::KeyCharEvent(const int keycode, const int unichar) noexcept
		:m_keycode(keycode), m_unichar(unichar)
	{
	}
}