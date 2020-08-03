///
/// Sprite.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Sprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	Sprite::Sprite()
	    : VertexData {}, Texture {}, Transform {}, m_opacity {1.0f}
	{
	}

	void Sprite::set_opacity(const pr::from_0_to_1 auto opacity) noexcept
	{
		m_opacity = opacity;
		m_dirty   = true;
	}

	void Sprite::set_z_level(const pr::positive_uint auto z_level) noexcept
	{
		m_z_level = z_level;
	}

	const float Sprite::opacity() const noexcept
	{
		return m_opacity;
	}

	void Sprite::bind() noexcept
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Sprite::unbind() noexcept
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} // namespace qs