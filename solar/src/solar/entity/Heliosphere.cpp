///
/// Heliosphere.cpp
///
/// solar
/// See LICENSE.txt.
///

#include "Heliosphere.hpp"

namespace sr
{
	Entity Heliosphere::create() noexcept
	{
		Entity e = euid::uid<Entity>();
		m_entities.insert(e);

		return e;
	}
}