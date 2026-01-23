///
/// EntityManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "EntityManager.hpp"

namespace galaxy
{
	EntityManager::EntityManager() noexcept
	{
	}

	EntityManager::EntityManager(EntityManager&& em) noexcept
	{
		this->m_registry = std::move(em.m_registry);
	}

	EntityManager& EntityManager::operator=(EntityManager&& em) noexcept
	{
		if (this != &em)
		{
			this->m_registry = std::move(em.m_registry);
		}

		return *this;
	}

	EntityManager::~EntityManager() noexcept
	{
	}

	Registry& EntityManager::registry() noexcept
	{
		return m_registry;
	}
} // namespace galaxy
