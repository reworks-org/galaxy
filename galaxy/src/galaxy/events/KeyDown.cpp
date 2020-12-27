///
/// KeyDown.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyDown.hpp"

namespace galaxy
{
	namespace events
	{
		KeyDown::KeyDown(const input::Keys keycode)
		    : m_keycode {keycode}
		{
		}
	} // namespace events
} // namespace galaxy