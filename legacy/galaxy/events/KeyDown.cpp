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
		KeyDown::KeyDown(const input::Keys keycode, const input::KeyMod mod) noexcept
			: m_keycode {keycode}
			, m_mod {mod}
		{
		}
	} // namespace events
} // namespace galaxy