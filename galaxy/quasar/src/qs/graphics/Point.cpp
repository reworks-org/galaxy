///
/// Point.cpp
/// quasar
///
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
		:m_size(0)
	{
	}

	Point::Point(const float x, const float y, const int size, pr::Colour& colour) noexcept
		:m_size(0)
	{
		create(x, y, size, colour);
	}

	void Point::create(const float x, const float y, const int size, pr::Colour& colour) noexcept
	{
		m_size = size;

		std::vector<qs::PrimitiveVertex> vertexs;
		IndexStorage indices;

		vertexs.emplace_back(qs::PrimitiveVertex{ x, y, colour });

		m_vertexBuffer.create<qs::PrimitiveVertex, qs::BufferTypeStatic>(vertexs);
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0 });

		m_layout.add<qs::PrimitiveVertex, qs::VATypePosition>(2);
		m_layout.add<qs::PrimitiveVertex, qs::VATypeColour>(4);

		m_vertexArray.create<qs::PrimitiveVertex>(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	void Point::bind() noexcept
	{
		m_vertexArray.bind();
	}

	void Point::unbind() noexcept
	{
		m_vertexArray.unbind();
	}

	const int Point::getSize() const noexcept
	{
		return m_size;
	}
}