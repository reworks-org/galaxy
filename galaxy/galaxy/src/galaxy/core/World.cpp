///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "World.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	World::World() noexcept
	{
	}

	World::~World() noexcept
	{
		m_manager.clear();
	}
}