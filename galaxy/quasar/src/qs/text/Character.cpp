///
/// Character.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Character.hpp"

///
/// Core namespace.
///
namespace qs
{
	Character::Character()
	    : VertexData {}, Texture {}, m_bearing_x {0}, m_bearing_y {0}, m_advance {0}
	{
	}

	Character::Character(const int bx, const int by, const unsigned int advance)
	    : VertexData {}, Texture {}, m_bearing_x {bx}, m_bearing_y {by}, m_advance {advance}
	{
	}

	Character::Character(Character&& c)
	{
		this->m_bearing_x = c.m_bearing_x;
		this->m_bearing_y = c.m_bearing_y;
		this->m_advance   = c.m_advance;
		this->m_region    = std::move(c.m_region);

		m_bearing_x = 0;
		m_bearing_y = 0;
		c.m_advance = 0;
	}

	Character& Character::operator=(Character&& c)
	{
		if (this != &c)
		{
			this->m_bearing_x = c.m_bearing_x;
			this->m_bearing_y = c.m_bearing_y;
			this->m_advance   = c.m_advance;
			this->m_region    = std::move(c.m_region);

			m_bearing_x = 0;
			m_bearing_y = 0;
			c.m_advance = 0;
		}

		return *this;
	}

	void Character::bind() noexcept
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Character::unbind() noexcept
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const unsigned int Character::get_advance() const noexcept
	{
		return m_advance;
	}

	const pr::Rect<float>& Character::get_region() const noexcept
	{
		return m_region;
	}
} // namespace qs