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
	
	void Sprite2D::addLayout(const unsigned int stride, const std::initializer_list<int>& strides) noexcept
	{
		m_layout.setStride(stride);
		for (const int& f : strides)
		{
			m_layout.add<float>(f);
		}
	}

	void Sprite2D::create() noexcept
	{
		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	void Sprite2D::activate() noexcept
	{
		m_vertexArray.bind();
		bind(); // texture
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