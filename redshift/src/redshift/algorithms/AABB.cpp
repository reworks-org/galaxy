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
		https://www.google.com/search?client=firefox-b-d&q=fast+algorithm+to+check+polygon+collision
	}

	AABB::AABB(const double topLeft, const double bottomRight) noexcept
		:m_topLeft(topLeft), m_bottomRight(bottomRight)
	{
	}
}