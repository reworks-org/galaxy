///
/// CircleCircle.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "CircleCircle.hpp"

///
/// Core namespace.
///
namespace rs
{
	const bool circleCircle(const rs::Circle& a, const rs::Circle& b)
	{
		return circleCircle(a.getCenter(), a.getRadius(), b.getCenter(), b.getRadius());
	}

	const bool circleCircle(const rs::Point& c1, const std::uint32_t r1, const rs::Point& c2, const std::uint32_t r2)
	{
		return ((c2.m_x - c1.m_x) ^ 2 + (c1.m_y - c2.m_y) ^ 2) <= ((r1 + r2) ^ 2);
	}
}