///
/// SystemManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

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
} // namespace galaxy
