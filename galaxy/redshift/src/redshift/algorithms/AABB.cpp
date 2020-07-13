///
/// AABB.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "AABB.hpp"

namespace rs
{
	const bool AABB(const pr::Rect<int>& a, const pr::Rect<int>& b) noexcept
	{
		// AABB algorithm
		return (a.m_x < b.m_x + b.m_width &&
			a.m_x + a.m_width > b.m_x &&
			a.m_y < b.m_y + b.m_height &&
			a.m_y + a.m_height > b.m_y);
	}
}