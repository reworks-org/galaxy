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
namespace pr
{
	KeyCharEvent::KeyCharEvent() noexcept
	    : m_keycode {pr::Keys::A}, m_unichar {0}
	{
	}

	KeyCharEvent::KeyCharEvent(const pr::Keys keycode, const int unichar) noexcept
	    : m_keycode {keycode}, m_unichar {unichar}
	{
	}
} // namespace pr