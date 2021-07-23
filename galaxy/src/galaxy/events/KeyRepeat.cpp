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
		KeyRepeat::KeyRepeat(const input::Keys keycode, const input::KeyMod mod) noexcept
			: m_keycode {keycode}
			, m_mod {mod}
		{
		}
	} // namespace events
} // namespace galaxy