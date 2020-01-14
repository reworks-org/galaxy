///
/// Rectangle.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Rectangle.hpp"

///
/// Core namespace.
///
namespace rs
{
	Rectangle::Rectangle() noexcept
		:m_topLeft(0.0), m_bottomRight(0.0)
	{
	}

	Rectangle::Rectangle(const double topLeft, const double bottomRight) noexcept
		:m_topLeft(topLeft), m_bottomRight(bottomRight)
	{
	}

	void Rectangle::setTopLeft(const double topLeft) noexcept
	{
		m_topLeft = topLeft;
	}

	void Rectangle::setBottomRight(const double bottomRight) noexcept
	{
		m_bottomRight = bottomRight;
	}

	const double Rectangle::getTopLeft() const noexcept
	{
		return m_topLeft;
	}

	const double Rectangle::getBottomRight() const noexcept
	{
		return m_bottomRight;
	}
}