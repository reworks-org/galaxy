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
	    : m_thickness {1.0f}
	{
	}

	Line::Line(const pr::Colour& col, const float x1, const float y1, const float x2, const float y2, const float thickness)
	    : m_thickness {thickness}
	{
		create(col, x1, y1, x2, y2, thickness);
	}

	void Line::create(const pr::Colour& col, const float x1, const float y1, const float x2, const float y2, const float thickness)
	{
		m_thickness = thickness;

		std::vector<qs::PrimitiveVertex> vertexs;
		IndexStorage indices;

		vertexs.emplace_back({x1, y1, col});
		vertexs.emplace_back({x2, y2, col});

		m_vb.create<qs::PrimitiveVertex, qs::BufferTypeStatic>(vertexs);
		m_ib.create<qs::BufferTypeStatic>({0, 1});

		m_layout.add<qs::PrimitiveVertex, qs::VATypePosition>(2);
		m_layout.add<qs::PrimitiveVertex, qs::VATypeColour>(4);

		m_va.create<qs::PrimitiveVertex>(m_vb, m_ib, m_layout);
	}

	void Line::bind() noexcept
	{
		m_va.bind();
		glLineWidth(m_thickness);
	}

	void Line::unbind() noexcept
	{
		m_va.unbind();
	}
} // namespace qs