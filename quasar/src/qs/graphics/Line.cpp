///
/// Line.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Line.hpp"

///
/// Core namespace.
///
namespace qs
{
	Line::Line() noexcept
		:m_thickness(1.0f), m_va(0), m_vb(0)
	{
	}

	Line::Line(const float x1, const float y1, const float x2, const float y2, const float thickness) noexcept
		:m_thickness(1.0f), m_va(0), m_vb(0)
	{
		create(x1, y1, x2, y2, thickness);
	}

	Line::~Line() noexcept
	{
		glDeleteVertexArrays(1, &m_va);
		glDeleteBuffers(1, &m_vb);
	}

	void Line::create(const float x1, const float y1, const float x2, const float y2, const float thickness) noexcept
	{
		m_thickness = thickness;

		glGenBuffers(1, &m_vb);
		glGenVertexArrays(1, &m_va);
		glBindVertexArray(m_va);

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);

		float vertexs[4] = { x1, y1, x2, y2 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Line::bind() noexcept
	{
		glBindVertexArray(m_va);
		glLineWidth(m_thickness);
	}

	void Line::unbind() noexcept
	{
		glBindVertexArray(0);
	}
}