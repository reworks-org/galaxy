///
/// Line.cpp
/// quasar
///
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
		:m_thickness(1.0f)
	{
	}

	Line::Line(pr::Colour& col, const float x1, const float y1, const float x2, const float y2, const float thickness) noexcept
		:m_thickness(1.0f)
	{
		create(col, x1, y1, x2, y2, thickness);
	}

	void Line::create(pr::Colour& col, const float x1, const float y1, const float x2, const float y2, const float thickness) noexcept
	{
		m_thickness = thickness;

		std::vector<qs::PrimitiveVertex> vertexs;
		IndexStorage indices;

		vertexs.emplace_back(qs::PrimitiveVertex{ x1, y1, col });
		vertexs.emplace_back(qs::PrimitiveVertex{ x2, y2, col });

		m_vertexBuffer.create<qs::PrimitiveVertex, qs::BufferTypeStatic>(vertexs);
		m_indexBuffer.create<qs::BufferTypeStatic>({0, 1});

		m_layout.add<qs::PrimitiveVertex, qs::VATypePosition>(2);
		m_layout.add<qs::PrimitiveVertex, qs::VATypeColour>(4);

		m_vertexArray.create<qs::PrimitiveVertex>(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	void Line::bind() noexcept
	{
		m_vertexArray.bind();
		glLineWidth(m_thickness);
	}

	void Line::unbind() noexcept
	{
		m_vertexArray.unbind();
	}
}