//
//  World.cpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "World.hpp"

namespace re
{
	World::~World()
	{
		// These two functions cleanup the databases.
		m_entityDatabase.clear();
		m_systemDatabase.clear();

		// Cleanup any leftover "dead" entitys.
		DeleteEntitys();
	}

	void World::AddEntity(const std::string& id, std::shared_ptr<Entity> entity)
	{
		// Add an entity to the unordered_map.
		entity->SetID(id);
		m_entityDatabase.emplace(id, entity);
	}

	void World::DeleteEntity(const std::string& id)
	{
		// Delete an entity from the database.
		m_entityDatabase.erase(id);
	}

	std::shared_ptr<Entity> World::GetEntity(const std::string& id)
	{
		return m_entityDatabase[id];
	}

	void World::RemoveEntity(const std::string& id)
	{
		m_entityDatabase[id]->m_isDead = true;

		std::shared_ptr<Entity> temp = m_entityDatabase[id];
		m_removedEntitys.emplace(id, temp);
		m_entityDatabase.erase(id);
	}

	void World::RestoreEntity(const std::string& id)
	{
		std::shared_ptr<Entity> temp = m_removedEntitys[id];
		m_entityDatabase.emplace(id, temp);
		m_removedEntitys.erase(id);

		m_entityDatabase[id]->m_isDead = false;
	}

	void World::DeleteEntitys()
	{
		for (auto& it : m_removedEntitys)
		{
			m_removedEntitys.erase(it.first);
		}

		m_removedEntitys.clear();
	}

	std::unordered_map<std::string, std::shared_ptr<Entity>>& World::GetEntityDatabase()
	{
		return m_entityDatabase;
	}

	std::unordered_map<std::type_index, std::shared_ptr<System>>& World::GetSystemDatabase()
	{
		return m_systemDatabase;
	}
}