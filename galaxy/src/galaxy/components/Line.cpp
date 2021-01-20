///
/// Line.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Line.hpp"

namespace galaxy
{
	namespace graphics
	{
		Line::Line(graphics::Colour& col, const float x1, const float y1, const float x2, const float y2)
		{
			create(col, x1, y1, x2, y2);
		}

		void Line::create(graphics::Colour& col, const float x1, const float y1, const float x2, const float y2)
		{
			std::vector<PrimitiveVertex> vertexs;
			vertexs.emplace_back(x1, y1, col);
			vertexs.emplace_back(x2, y2, col);

			m_vb.create<PrimitiveVertex, BufferStatic>(vertexs);

			std::array<unsigned int, 2> arr = {0, 1};
			m_ib.create<BufferStatic>(arr);

			m_layout.add<PrimitiveVertex, VAPosition>(2);
			m_layout.add<PrimitiveVertex, VAColour>(4);

			m_va.create<PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Line::bind()
		{
			m_va.bind();
		}

		void Line::unbind()
		{
			m_va.unbind();
		}
	} // namespace graphics
} // namespace galaxy