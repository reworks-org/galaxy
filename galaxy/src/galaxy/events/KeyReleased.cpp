///
/// KeyReleased.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "KeyReleased.hpp"

namespace galaxy
{
	KeyReleased::KeyReleased(const SDL_Scancode scancode, const Keys key, const KeyMods mod, const unsigned short raw, const bool repeat) noexcept
		: m_scancode {scancode}
		, m_key {key}
		, m_mod {mod}
		, m_raw {raw}
		, m_repeat {repeat}
	{
	}
} // namespace galaxy
