///
/// Collidable.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Collidable.hpp"

///
/// Core namespace.
///
namespace rs
{
	AABB& Collidable::get_aabb()
	{
		return m_aabb;
	}
} // namespace rs