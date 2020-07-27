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
	    : VertexData {}, Texture {}, m_bearingX {0}, m_bearingY {0}, m_advance {0}
	{
	}

	Character::Character(const int bx, const int by, const pr::positive_uint auto advance)
	    : VertexData {}, Texture {}, m_bearingX {bx}, m_bearingY {by}, m_advance {advance}
	{
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