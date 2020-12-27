///
/// Point.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Point.hpp"

namespace galaxy
{
	namespace graphics
	{
		Point::Point()
		    : m_size {0}
		{
		}

		Point::Point(const float x, const float y, const unsigned int size, graphics::Colour& colour)
		    : m_size {0}
		{
			create(x, y, size, colour);
		}

		void Point::create(const float x, const float y, const unsigned int size, graphics::Colour& colour)
		{
			m_size = size;

			std::vector<PrimitiveVertex> vertexs;

			vertexs.emplace_back(x, y, colour);

			m_vb.create<PrimitiveVertex, BufferStatic>(vertexs);

			std::array<unsigned int, 1> arr = {0};
			m_ib.create<BufferStatic>(arr);

			m_layout.add<PrimitiveVertex, VAPosition>(2);
			m_layout.add<PrimitiveVertex, VAColour>(4);

			m_va.create<PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Point::bind()
		{
			m_va.bind();
		}

		void Point::unbind()
		{
			m_va.unbind();
		}

		const int Point::get_size() const
		{
			return m_size;
		}
	} // namespace graphics
} // namespace galaxy