///
/// InstancedSprite.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cstdarg>

#include "qs/utils/Utility.hpp"

#include "InstancedSprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	InstancedSprite::InstancedSprite() noexcept
		:Texture(), Transform()
	{
	}

	InstancedSprite::~InstancedSprite() noexcept
	{
		unbind();
		glDeleteTextures(1, &m_id);
	}

	void InstancedSprite::setOpacity(float opacity) noexcept
	{
		if (opacity > 1.0f)
		{
			opacity = 1.0f;
		}
		else if (opacity < 0.0f)
		{
			opacity = 0.0f;
		}

		auto& vs = m_vertexBuffer.getVertexs();
		vs[0].m_colour[3] = opacity;
		vs[1].m_colour[3] = opacity;
		vs[2].m_colour[3] = opacity;
		vs[3].m_colour[3] = opacity;
	}

	void InstancedSprite::bind() noexcept
	{
		m_vertexArray.bind();
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void InstancedSprite::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const unsigned int InstancedSprite::getCount() const noexcept
	{
		return m_indexBuffer.getCount();
	}
}