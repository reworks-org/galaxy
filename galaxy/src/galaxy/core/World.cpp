///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "World.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	World::World()
	{
		// Register default components.
		// TODO
	}

	World::~World()
	{
		m_componentFactory.clear();
		m_manager.clear();
	}

	sr::Entity World::createEntity(const std::string& def)
	{
		galaxy::FileSystem* fs = galaxy::ServiceLocator::i().fs();

		nlohmann::json root;
		root.parse(fs->read(def));

		// Create entity.
		sr::Entity entity = m_manager.create();

		// Loop over components
		if (!root.empty())
		{
			for (auto& [key, value] : root.items())
			{
				// Use the assign function to create components for entities without having to know the type.
				m_componentFactory[key](entity, value);
			}
		}

		return entity;
	}

	void World::createEntities(const std::string& def)
	{	
		galaxy::FileSystem* fs = galaxy::ServiceLocator::i().fs();

		nlohmann::json root;
		root.parse(fs->read(def));

		for (auto& location : root)
		{
			createEntity(location);
		}
	}

	void World::createDuplicateEntities(const unsigned int count, const std::string& def)
	{
		for (auto i = 0; i < count; ++i)
		{
			createEntity(def);
		}
	}

	void World::update(const double dt)
	{
		// Update ECS.
		m_manager.update(dt);
	}

	sr::Manager& World::manager() noexcept
	{
		return m_manager;
	}
}