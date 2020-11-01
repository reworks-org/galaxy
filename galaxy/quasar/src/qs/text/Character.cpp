///
/// Character.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Character.hpp"

///
/// Core namespace.
///
namespace qs
{
	Character::Character()
	    : m_gl_texture {0}, m_size {0, 0}, m_bearing {0, 0}, m_advance {0}
	{
		glGenTextures(1, &m_gl_texture);
	}

	Character::Character(Character&& c)
	{
		this->m_gl_texture = c.m_gl_texture;
		this->m_size       = std::move(c.m_size);
		this->m_bearing    = std::move(c.m_bearing);
		this->m_advance    = c.m_advance;

		c.m_gl_texture = 0;
	}

	Character& Character::operator=(Character&& c)
	{
		if (this != &c)
		{
			this->m_gl_texture = c.m_gl_texture;
			this->m_size       = std::move(c.m_size);
			this->m_bearing    = std::move(c.m_bearing);
			this->m_advance    = c.m_advance;

			c.m_gl_texture = 0;
		}

		return *this;
	}

	Character::~Character()
	{
		glDeleteTextures(1, &m_gl_texture);
	}
} // namespace qs