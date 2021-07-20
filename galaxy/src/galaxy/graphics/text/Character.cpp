///
/// Character.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Character.hpp"

namespace galaxy
{
	namespace graphics
	{
		Character::Character() noexcept
			: m_texture {0}
			, m_size {0, 0}
			, m_bearing {0, 0}
			, m_advance {0}
		{
			glGenTextures(1, &m_texture);
		}

		Character::Character(Character&& c) noexcept
		{
			this->m_texture = c.m_texture;
			this->m_size    = std::move(c.m_size);
			this->m_bearing = std::move(c.m_bearing);
			this->m_advance = c.m_advance;

			c.m_texture = 0;
		}

		Character& Character::operator=(Character&& c) noexcept
		{
			if (this != &c)
			{
				this->m_texture = c.m_texture;
				this->m_size    = std::move(c.m_size);
				this->m_bearing = std::move(c.m_bearing);
				this->m_advance = c.m_advance;

				c.m_texture = 0;
			}

			return *this;
		}

		Character::~Character() noexcept
		{
			glDeleteTextures(1, &m_texture);
		}
	} // namespace graphics
} // namespace galaxy