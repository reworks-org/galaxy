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
namespace pr
{
	KeyUpEvent::KeyUpEvent(const pr::Keys keycode) noexcept
		:m_keycode(keycode)
	{
	}
}