///
/// Renderable.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Renderable.hpp"

///
/// Core namespace.
///
namespace qs
{
	void Renderable::bind() noexcept
	{
		m_vertexArray.bind();
		glBindTexture(GL_TEXTURE_2D, m_textureHandle);
	}

	void Renderable::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	const unsigned int Renderable::getCount() const noexcept
	{
		return m_indexBuffer.getCount();
	}
}