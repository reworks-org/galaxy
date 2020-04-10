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
		m_vertexArray.unbind();
		unbind(); // texture
		glDeleteTextures(1, &m_id);
	}
	
	void Sprite2D::create() noexcept
	{
		// Create basic array of verticies.
		m_vertexBuffer.create<16>({
			0.0f, 0.0f, 0.0f, 1.0f,
			(float)getWidth(), 0.0f, 1.0f, 1.0f,
			(float)getWidth(), (float)getHeight(), 1.0f, 0.0f,
			0.0f, (float)getHeight(), 0.0f, 0.0f
		});

		m_indexBuffer.create<6>({
			0, 1, 3,
			1, 2, 3
		});

		m_layout.setStride(4);
		m_layout.add<float>(2);
		m_layout.add<float>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		setRotationOrigin(getWidth() * 0.5f, getHeight() * 0.5f);
	}

	void Sprite2D::activate() noexcept
	{
		bind(); // texture
		m_vertexArray.bind();
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