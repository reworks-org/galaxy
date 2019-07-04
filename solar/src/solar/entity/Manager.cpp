///
/// Manager.cpp
///
/// solar
/// See LICENSE.txt.
///

#include "Manager.hpp"

namespace sr
{
	Manager::Manager()
		:m_nextID(0)
	{
	}

	Manager::~Manager()
	{
		m_data.clear();
		m_entities.clear();

		m_nextID = 0;
	}

	Entity Manager::create() noexcept
	{
		Entity e = m_nextID++;

		m_entities.insert(e);

		return e;
	}

	void Manager::destroy(Entity entity)
	{
		for (auto&& ptr : m_data)
		{
			ptr->remove(entity);
		}
	}

	void Manager::event(const Event& event)
	{
		for (auto&& ptr : m_systems)
		{
			ptr->event(event);
		}
	}

	void Manager::update(const DeltaTime time)
	{
		for (auto&& ptr : m_systems)
		{
			ptr->update(time, *this);
		}
	}
}