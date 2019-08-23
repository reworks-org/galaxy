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
		// Bit masks the unsigned interger id with the valid_entity flag
		// to make sure when an unsigned integer is passed it can be
		// verified as an actual entity.
		SR_INTEGER free = m_nextID++;
		sr::Entity entity = free << 16 | sr::VALID_ENTITY;
		
		m_entities.insert(entity);

		return entity;
	}

	bool Manager::has(sr::Entity entity) noexcept
	{
		bool result = true;

		// Do not need to verify since parameter wont match unless entity flag is present.
		(m_entities.has(entity)) ? result = true : result = false;

		return result;
	}

	bool Manager::validate(sr::Entity entity)
	{
		bool result = true;

		// Checks if flag exists.
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