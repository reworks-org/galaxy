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
	    : m_next_id {0}
	{
	}

	Manager::~Manager()
	{
		clear();
	}

	const sr::Entity Manager::create()
	{
		// Bit masks the unsigned interger id with the valid_entity flag
		// to make sure when an unsigned integer is passed it can be
		// verified as an actual entity.
		SR_INTEGER entity = 0;
		if (!m_invalid_entities.empty())
		{
			const auto end = m_invalid_entities.size() - 1;
			entity         = m_invalid_entities[end];
			m_invalid_entities.pop_back();
		}
		else
		{
			entity = m_next_id++;
		}

		m_entities.insert(entity);
		return entity;
	}

	const sr::Entity Manager::create(std::string_view debug_name)
	{
		const auto entity = create();
		assign_name(entity, debug_name);

		return entity;
	}

	const bool Manager::has(const sr::Entity entity) noexcept
	{
		return m_entities.has(entity);
	}

	const bool Manager::validate(const sr::Entity entity)
	{
		return (std::find(m_invalid_entities.begin(), m_invalid_entities.end(), entity) == m_invalid_entities.end());
	}

	bool Manager::assign_name(const sr::Entity entity, std::string_view debug_name)
	{
		if (validate(entity))
		{
			const auto str = static_cast<std::string>(debug_name);
			if (!m_debug_names.contains(str))
			{
				m_debug_names.emplace(str, entity);
				return true;
			}
			else
			{
				PL_LOG(PL_WARNING, "Attempted to add duplicate name: {0}.", str);
				return false;
			}
		}
		else
		{
			PL_LOG(PL_ERROR, "Attempted to use invalid entity to assign name: {0}.", debug_name);
			return false;
		}
	}

	sr::Entity Manager::find_from_name(std::string_view debug_name)
	{
		const auto str = static_cast<std::string>(debug_name);
		if (m_debug_names.contains(str))
		{
			return m_debug_names[str];
		}
		else
		{
			PL_LOG(PL_ERROR, "Cannot find entity to get with a name of: {0}.", str);
			return 0;
		}
	}

	auto Manager::get_debug_name_map() noexcept -> const std::unordered_map<std::string, sr::Entity>&
	{
		return m_debug_names;
	}

	void Manager::destroy(const sr::Entity entity)
	{
		m_entities.remove(entity);

		for (auto&& ptr : m_data)
		{
			ptr->remove(entity);
		}

		m_invalid_entities.push_back(entity);

		std::erase_if(m_debug_names, [&](const auto& pair) {
			return pair.second == entity;
		});
	}

	void Manager::events()
	{
		for (auto&& ptr : m_systems)
		{
			ptr->events();
		}
	}

	void Manager::update(const double dt)
	{
		for (auto&& ptr : m_systems)
		{
			ptr->update(dt);
		}
	}

	void Manager::clear()
	{
		m_next_id = 0;

		m_entities.clear();
		m_data.clear();
		m_systems.clear();
		m_debug_names.clear();
	}
} // namespace sr