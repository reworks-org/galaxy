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
	const bool circleCircle(const protostar::Circle<int>& a, const protostar::Circle<int>& b)
	{
		return ((b.getCenter().m_x - a.getCenter().m_x) ^ 2 + (a.getCenter().m_y - b.getCenter().m_y) ^ 2) <= ((a.getRadius() + b.getRadius()) ^ 2);
	}
}