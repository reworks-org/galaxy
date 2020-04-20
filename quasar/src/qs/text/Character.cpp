///
/// Character.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Character.hpp"

///
/// Core namespace.
///
namespace qs
{
	Character::Character() noexcept
		:m_id(0), m_width(0), m_height(0), m_bearingX(0), m_bearingY(0), m_advance(0)
	{
	}

	Character::Character(const unsigned int id, const int w, const int h, const int bx, const int by, const unsigned int advance) noexcept
		:m_id(id), m_width(w), m_height(h), m_bearingX(bx), m_bearingY(by), m_advance(advance)
	{
	}
}