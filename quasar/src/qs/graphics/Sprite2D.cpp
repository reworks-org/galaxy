///
/// Sprite2D.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cstdarg>

#include "Sprite2D.hpp"

///
/// Core namespace.
///
namespace qs
{
	Sprite2D::Sprite2D() noexcept
		:Transform(), Texture()
	{
	}

	Sprite2D::~Sprite2D() noexcept
	{
		unbind();
		glDeleteTextures(1, &m_id);
	}
	
	void Sprite2D::create() noexcept
	{
		m_vertexBuffer.create<16>({
			0.0f, 0.0f, 0.0f, 1.0f,
			static_cast<float>(m_width), 0.0f, 1.0f, 1.0f,
			static_cast<float>(m_width), static_cast<float>(m_height), 1.0f, 0.0f,
			0.0f, static_cast<float>(m_height), 0.0f, 0.0f
		});

		m_indexBuffer.create<6>({
			0, 1, 3,
			1, 2, 3
		});

		m_layout.setStride(4);
		m_layout.add<float>(2);
		m_layout.add<float>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		setRotationOrigin(static_cast<float>(m_width) * 0.5f, static_cast<float>(m_height) * 0.5f);
	}

	void Sprite2D::bind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		m_vertexArray.bind();
	}

	void Sprite2D::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	qs::VertexArray& Sprite2D::getVAO() noexcept
	{
		return m_vertexArray;
	}

	qs::IndexBuffer& Sprite2D::getIBO() noexcept
	{
		return m_indexBuffer;
	}
}