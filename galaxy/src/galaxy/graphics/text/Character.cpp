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
		    : m_gl_texture {0}, m_size {0, 0}, m_bearing {0, 0}, m_advance {0}, m_region {0.0f, 0.0f, 0.0f, 0.0f}
		{
			glGenTextures(1, &m_gl_texture);
		}

		Character::Character(Character&& c) noexcept
		{
			this->m_gl_texture = c.m_gl_texture;
			this->m_size       = std::move(c.m_size);
			this->m_bearing    = std::move(c.m_bearing);
			this->m_advance    = c.m_advance;
			this->m_region     = std::move(c.m_region);

			c.m_gl_texture = 0;
		}

		Character& Character::operator=(Character&& c) noexcept
		{
			if (this != &c)
			{
				this->m_gl_texture = c.m_gl_texture;
				this->m_size       = std::move(c.m_size);
				this->m_bearing    = std::move(c.m_bearing);
				this->m_advance    = c.m_advance;
				this->m_region     = std::move(c.m_region);

				c.m_gl_texture = 0;
			}

			return *this;
		}

		Character::~Character() noexcept
		{
			glDeleteTextures(1, &m_gl_texture);
		}
	} // namespace graphics
} // namespace galaxy