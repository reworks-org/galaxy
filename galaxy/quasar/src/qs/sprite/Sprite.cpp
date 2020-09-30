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

	void Sprite::set_opacity(const float opacity) noexcept
	{
		if (m_opacity > 1.0f)
		{
			m_opacity = 1.0f;
		}
		else if (m_opacity < 0.0f)
		{
			m_opacity = 0.0f;
		}
		else
		{
			m_opacity = opacity;
		}

		m_dirty = true;
	}

	void Sprite::set_z_level(const unsigned int z_level) noexcept
	{
		m_z_level = z_level;
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

	const float Sprite::opacity() const noexcept
	{
		return m_opacity;
	}
} // namespace qs