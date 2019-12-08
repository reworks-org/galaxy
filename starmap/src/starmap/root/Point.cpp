///
/// Point.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "Point.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Point::Point() noexcept
		:m_x(0.0), m_y(0.0)
	{
	}

	Point::Point(const double x, const double y) noexcept
		:m_x(x), m_y(y)
	{
	}

	void Point::setX(const double x) noexcept
	{
		m_x = x;
	}

	void Point::setY(const double y) noexcept
	{
		m_y = y;
	}

	const double Point::getX() const noexcept
	{
		return m_x;
	}

	const double Point::getY() const noexcept
	{
		return m_y;
	}
}