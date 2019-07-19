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
		SR_INTEGER free = m_nextID++;
		sr::Entity entity = free << 16 | sr::VALID_ENTITY;
		
		m_entities.insert(entity);

		return entity;
	}

	bool Manager::has(sr::Entity entity) noexcept
	{
		bool result = true;

		// may need to validate input to see if entity requires testing...

		(m_entities.has(entity)) ? result = true : result = false;

		return result;
	}

	bool Manager::validate(sr::Entity entity)
	{
		bool result = true;

		((entity & 0xFFFF) == sr::VALID_ENTITY) ? result = true : result = false;

		return result;
	}

	void Manager::destroy(Entity entity)
	{
		m_entities.remove(entity);

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