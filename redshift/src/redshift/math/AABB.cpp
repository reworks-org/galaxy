///
/// AABB.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "AABB.hpp"

namespace rs
{
	AABB::AABB() noexcept
		:m_topLeft(0.0), m_bottomRight(0.0)
	{
	}

	AABB::AABB(const double topLeft, const double bottomRight) noexcept
		:m_topLeft(topLeft), m_bottomRight(bottomRight)
	{
	}
}