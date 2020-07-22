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
namespace pr
{
	KeyDownEvent::KeyDownEvent(const pr::Keys keycode) noexcept
	    : m_keycode {keycode}
	{
	}
} // namespace pr