///
/// KeyRepeat.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyRepeat.hpp"

namespace galaxy
{
	namespace events
	{
		KeyRepeat::KeyRepeat(const input::Keys keycode) noexcept
			: m_keycode {keycode}
		{
		}
	} // namespace events
} // namespace galaxy