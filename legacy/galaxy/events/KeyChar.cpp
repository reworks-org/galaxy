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
		KeyChar::KeyChar() noexcept
			: m_character {'\0'}
		{
		}

		KeyChar::KeyChar(char character) noexcept
		{
			m_character = character;
		}
	} // namespace events
} // namespace galaxy