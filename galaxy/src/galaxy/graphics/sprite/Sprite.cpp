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
	    : VertexData {}, Texture {}, Transform {}
	{
	}

	void Sprite::bind()
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Sprite::unbind()
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} // namespace qs