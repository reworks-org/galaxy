///
/// KeyChar.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyChar.hpp"

namespace galaxy
{
	namespace events
	{
		KeyChar::KeyChar()
		    : m_keycode {input::Keys::A}, m_unichar {0}
		{
		}

		KeyChar::KeyChar(const input::Keys keycode, const int unichar)
		    : m_keycode {keycode}, m_unichar {unichar}
		{
		}
	} // namespace events
} // namespace galaxy