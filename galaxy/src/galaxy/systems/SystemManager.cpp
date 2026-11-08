///
/// SystemManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "SystemManager.hpp"

namespace galaxy
{
	SystemManager::SystemManager() noexcept
		: m_suspended {false}
	{
	}

	SystemManager::SystemManager(SystemManager&& sm) noexcept
	{
		this->m_systems   = std::move(sm.m_systems);
		this->m_suspended = sm.m_suspended;
	}

	SystemManager& SystemManager::operator=(SystemManager&& sm) noexcept
	{
		if (this != &sm)
		{
			this->m_systems   = std::move(sm.m_systems);
			this->m_suspended = sm.m_suspended;
		}

		return *this;
	}

	SystemManager::~SystemManager() noexcept
	{
	}

	void SystemManager::add_system(const std::string& system)
	{
		auto& sf = entt::locator<SystemFactory>::value();
		sf.create_system(system, m_systems);
	}

	void SystemManager::update(EntityManager& em, Scene* scene)
	{
		if (!m_suspended)
		{
			for (auto&& system : m_systems)
			{
				system->update(em, scene);
			}
		}
	}

	void SystemManager::suspend() noexcept
	{
		m_suspended = true;
	}

	void SystemManager::resume() noexcept
	{
		m_suspended = false;
	}

	void SystemManager::clear()
	{
		m_systems.clear();
	}

	SystemStack& SystemManager::stack() noexcept
	{
		return m_systems;
	}

	bool SystemManager::is_suspended() const noexcept
	{
		return m_suspended;
	}

	nlohmann::json SystemManager::serialize()
	{
		nlohmann::json json = "{}"_json;

		json["systems"]         = nlohmann::json::object();
		json["systems"]["list"] = nlohmann::json::object();

		for (auto i = 0; i < m_systems.size(); i++)
		{
			json["systems"]["list"][std::to_string(i)] = m_systems[i]->id();
		}

		json["systems"]["suspended"] = m_suspended;

		return json;
	}

	void SystemManager::deserialize(const nlohmann::json& json)
	{
		const auto& systems = json.at("systems");

		m_systems.reserve(systems.size());
		for (const auto& [index, name] : systems.items())
		{
			add_system(name.get<std::string>());
		}

		m_suspended = json.at("suspended");
	}
} // namespace galaxy
