///
/// Point.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Point.hpp"

///
/// Core namespace.
///
namespace qs
{
	Point::Point() noexcept
		:m_size(0), m_va(0), m_vb(0)
	{
	}

	Point::Point(const float x, const float y, const int size) noexcept
		:m_size(0), m_va(0), m_vb(0)
	{
		create(x, y, size);
	}

	Point::~Point() noexcept
	{
		glDeleteVertexArrays(1, &m_va);
		glDeleteBuffers(1, &m_vb);
	}

	void Point::create(const float x, const float y, const int size) noexcept
	{
		m_size = size;

		glGenBuffers(1, &m_vb);
		glGenVertexArrays(1, &m_va);
		glBindVertexArray(m_va);

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);

		float vertex[2] = { x, y };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Point::bind() noexcept
	{
		glBindVertexArray(m_va);
	}

	void Point::unbind() noexcept
	{
		glBindVertexArray(0);
	}

	const int Point::getSize() const noexcept
	{
		return m_size;
	}
}