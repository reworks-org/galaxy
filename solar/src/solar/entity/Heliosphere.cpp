///
/// Heliosphere.cpp
///
/// solar
/// See LICENSE.txt.
///

#include "Heliosphere.hpp"

namespace sr
{
	Heliosphere::Heliosphere()
		:m_nextID(0)
	{
	}

	Heliosphere::~Heliosphere()
	{
		m_entities.clear();
		m_components.clear();
		m_systems.clear();

		m_nextID = 0;
	}

	Entity Heliosphere::create() noexcept
	{
		Entity e = m_nextID++;
		m_entities.insert(e);

		return e;
	}

	void Heliosphere::destroy(Entity entity)
	{
		for (auto& ptr : m_data)
		{
			ptr->
		}
	}
}