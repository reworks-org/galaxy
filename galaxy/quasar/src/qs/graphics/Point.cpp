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
	Point::Point() noexcept
	    : m_size {0}
	{
	}

	Point::Point(const float x, const float y, const int size, const pr::Colour& colour)
	    : m_size {0}
	{
		create(x, y, size, colour);
	}

	void Point::create(const float x, const float y, const int size, const pr::Colour& colour)
	{
		m_size = size;

		std::vector<qs::PrimitiveVertex> vertexs;
		IndexStorage indices;

		vertexs.emplace_back({x, y, colour});

		m_vb.create<qs::PrimitiveVertex, qs::BufferTypeStatic>(vertexs);
		m_ib.create<qs::BufferTypeStatic>({0});

		m_layout.add<qs::PrimitiveVertex, qs::VATypePosition>(2);
		m_layout.add<qs::PrimitiveVertex, qs::VATypeColour>(4);

		m_va.create<qs::PrimitiveVertex>(m_vb, m_ib, m_layout);
	}

	void Point::bind() noexcept
	{
		m_va.bind();
	}

	void Point::unbind() noexcept
	{
		m_va.unbind();
	}

	const int Point::get_size() const noexcept
	{
		return m_size;
	}
} // namespace qs