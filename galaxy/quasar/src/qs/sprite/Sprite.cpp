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
	Sprite::Sprite() noexcept
		:VertexData(), Texture(), Transform(), m_opacity(1.0f)
	{
	}

	void Sprite::setOpacity(float opacity) noexcept
	{
		if (opacity > 1.0f)
		{
			opacity = 1.0f;
		}
		else if (opacity < 0.0f)
		{
			opacity = 0.0f;
		}

		m_opacity = opacity;
	}

	const float Sprite::getOpacity() const noexcept
	{
		return m_opacity;
	}

	void Sprite::bind() noexcept
	{
		m_vertexArray.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Sprite::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}