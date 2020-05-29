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
	Character::Character() noexcept
		:VertexData(), Texture(), m_bearingX(0), m_bearingY(0), m_advance(0)
	{
	}

	Character::Character(const int bx, const int by, const unsigned int advance) noexcept
		:VertexData(), Texture(), m_bearingX(bx), m_bearingY(by), m_advance(advance)
	{
	}

	void Character::bind() noexcept
	{
		m_vertexArray.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Character::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const unsigned int Character::getAdvance() const noexcept
	{
		return m_advance;
	}

	const protostar::Rect<float>& Character::getRegion() const noexcept
	{
		return m_region;
	}
}