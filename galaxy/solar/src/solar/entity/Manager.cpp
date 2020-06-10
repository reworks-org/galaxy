///
/// Manager.cpp
///
/// solar
/// See LICENSE.txt.
///

#include "Manager.hpp"

namespace sr
{
	Manager::Manager() noexcept
		:m_nextID(0)
	{
	}

	Manager::~Manager() noexcept
	{
		clear();
	}

	const sr::Entity Manager::create() noexcept
	{
		// Bit masks the unsigned interger id with the valid_entity flag
		// to make sure when an unsigned integer is passed it can be
		// verified as an actual entity.
		SR_INTEGER entity = 0;
		if (!m_invalidEntities.empty())
		{
			auto end = m_invalidEntities.size() - 1;
			entity = m_invalidEntities[end];
			m_invalidEntities.pop_back();
		}
		else
		{
			SR_INTEGER entity = m_nextID++;
		}

		m_entities.insert(entity);
		return entity;
	}

	const bool Manager::has(const sr::Entity entity) noexcept
	{
		// Do not need to verify since parameter wont match unless entity flag is present.
		return m_entities.has(entity);
	}

	const bool Manager::validate(const sr::Entity entity) noexcept
	{
		return (entity != std::numeric_limits<sr::Entity>::max()) && 
			   (std::find(m_invalidEntities.begin(), m_invalidEntities.end(), entity) == m_invalidEntities.end());
	}

	void Manager::destroy(const sr::Entity entity) noexcept
	{
		m_entities.remove(entity);

		for (auto&& ptr : m_data)
		{
			ptr->remove(entity);
		}

		m_invalidEntities.push_back(entity);
	}

	void Manager::events() noexcept
	{
		for (auto&& ptr : m_systems)
		{
			ptr->events();
		}
	}

	void Manager::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
		for (auto&& ptr : m_systems)
		{
			ptr->update(deltaTime, *this);
		}
	}

	void Manager::clear() noexcept
	{
		m_nextID = 0;

		m_entities.clear();
		m_data.clear();
		m_systems.clear();
	}
}