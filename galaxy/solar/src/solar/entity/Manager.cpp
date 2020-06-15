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
			entity = m_nextID++;
		}

		m_entities.insert(entity);
		return entity;
	}

	const sr::Entity Manager::create(const std::string& debugName) noexcept
	{
		auto entity = create();
		assignName(entity, debugName);

		return entity;
	}

	void Manager::assignName(const sr::Entity entity, const std::string& debugName) noexcept
	{
		if (validate(entity))
		{
			if (m_debugNames.find(debugName) == m_debugNames.end())
			{
				m_debugNames.emplace(debugName, entity);
			}
			else
			{
				PL_LOG(PL_WARNING, "Attempted to add duplicate name: " + debugName);
			}
		}
		else
		{
			PL_LOG(PL_ERROR, "Attempted to use invalid entity.");
		}
	}

	sr::Entity Manager::findFromName(const std::string& debugName) noexcept
	{
		if (m_debugNames.find(debugName) != m_debugNames.end())
		{
			return m_debugNames[debugName];
		}
		else
		{
			PL_LOG(PL_ERROR, "Cannot find entity to get with a name of: " + debugName);
			return 0;
		}
	}

	const std::unordered_map<std::string, sr::Entity>& Manager::getAllNames() noexcept
	{
		return m_debugNames;
	}

	const bool Manager::has(const sr::Entity entity) noexcept
	{
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
		
		auto pos = std::find_if(m_debugNames.begin(), m_debugNames.end(), [&](auto& pair)
		{
			return pair.second == entity;
		});

		if (pos != m_debugNames.end())
		{
			m_debugNames.erase(pos);
		}
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
			ptr->update(deltaTime);
		}
	}

	void Manager::clear() noexcept
	{
		m_nextID = 0;

		m_entities.clear();
		m_data.clear();
		m_systems.clear();
		m_debugNames.clear();
	}
}