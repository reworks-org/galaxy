///
/// Collidable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Collidable.hpp"

namespace galaxy
{
	namespace physics
	{
		AABB& Collidable::get_aabb() noexcept
		{
			return m_aabb;
		}
	} // namespace physics
} // namespace galaxy