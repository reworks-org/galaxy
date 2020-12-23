///
/// Point.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Point.hpp"

///
/// Core namespace.
///
namespace qs
{
	Point::Point()
	    : m_size {0}
	{
	}

	Point::Point(const float x, const float y, const unsigned int size, pr::Colour& colour)
	    : m_size {0}
	{
		create(x, y, size, colour);
	}

	void Point::create(const float x, const float y, const unsigned int size, pr::Colour& colour)
	{
		m_size = size;

		std::vector<qs::PrimitiveVertex> vertexs;

		vertexs.emplace_back(x, y, colour);

		m_vb.create<qs::PrimitiveVertex, qs::BufferStatic>(vertexs);

		std::array<unsigned int, 1> arr = {0};
		m_ib.create<qs::BufferStatic>(arr);

		m_layout.add<qs::PrimitiveVertex, qs::VAPosition>(2);
		m_layout.add<qs::PrimitiveVertex, qs::VAColour>(4);

		m_va.create<qs::PrimitiveVertex>(m_vb, m_ib, m_layout);
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
} // namespace qs