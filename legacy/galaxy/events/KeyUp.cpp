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
		KeyUp::KeyUp(const input::Keys keycode, const input::KeyMod mod) noexcept
			: m_keycode {keycode}
			, m_mod {mod}
		{
		}
	} // namespace events
} // namespace galaxy