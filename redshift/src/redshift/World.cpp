///
/// World.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "World.hpp"

namespace rs
{
	World::World()
		:m_gravity(9.81)
	{
	}

	World::~World()
	{
	}

	void World::changeGravity(const double gravity) noexcept
	{
		m_gravity = gravity;
	}
}