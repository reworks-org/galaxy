///
/// KeyUp.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyUp.hpp"

namespace galaxy
{
	namespace events
	{
		KeyUp::KeyUp(const input::Keys keycode)
		    : m_keycode {keycode}
		{
		}
	} // namespace events
} // namespace galaxy