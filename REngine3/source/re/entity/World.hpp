//
//  World.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WORLD_HPP_
#define RENGINE3_WORLD_HPP_

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "re/entity/Entity.hpp"
#include "re/systems/System.hpp"
#include "re/services/Service.hpp"

namespace re
{
	class World : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up world.
		*/
		~World() override;

		/*
		* IMPORTS: id - assign an id to the entity
		*          e - the shared_ptr to the entity. use 'new' to create.
		* EXPORTS: none
		* PURPOSE: To create a new entity and add it to the world.
		*/
		void AddEntity(const std::string& id, std::shared_ptr<Entity> entity);

		/*
		* IMPORTS: id - ID of the entity to delete.
		* EXPORTS: none
		* PURPOSE: Deletes an entity from the world permanently. Recreate with Create().
		*/
		void DeleteEntity(const std::string& id);

		/*
		* IMPORTS: id - ID of the entity to retrieve.
		* EXPORTS: a shared_ptr to the entity.
		* PURPOSE: To retrieve an entity from the world.
		*/
		std::shared_ptr<Entity> GetEntity(const std::string& id);

		/*
		* IMPORTS: id - The ID of the entity.
		* EXPORTS: none
		* PURPOSE: Remove an entity from the world but without deleting it. Stored in m_removedEntitys database.
		*/
		void RemoveEntity(const std::string& id);

		/*
		* IMPORTS: id - The ID of the entity.
		* EXPORTS: none
		* PURPOSE: Restore an entity from the killed database to the world database. m_removedEntitys -> m_entityDatabase.
		*/
		void RestoreEntity(const std::string& id);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Deletes all entitys in m_removedEntitys.
		*/
		void DeleteEntitys();

		/*
		* IMPORTS: s - The system to create. This uses polymorphism. Define the type of system being created with the template.
		REMEMBER TO USE 'new' and the correct system type!
		* EXPORTS: none
		* PURPOSE: To create a system that operates on the entities and add it to the world. You can only have one of each system.
		*/
		template<typename T>
		void AddSystem(std::unique_ptr<System> system);

		/*
		* IMPORTS: template - Define the system to use through the template type.
		* EXPORTS: none
		* PURPOSE: To delete a system from the world.
		*/
		template<typename T>
		void DeleteSystem();

		/*
		* IMPORTS: template - use the type of system to retrieve that system.
		* EXPORTS: shared_ptr to that system.
		* PURPOSE: To retrieve a system from the world.
		*/
		template<typename T>
		std::shared_ptr<T> GetSystem();

		/*
		* IMPORTS: none
		* EXPORTS: unordered_map of entitys in world.
		* PURPOSE: To retrieve a map of entitys in the world. ID is the key.
		*/
		std::unordered_map<std::string, std::shared_ptr<Entity>>& GetEntityDatabase();

		/*
		* IMPORTS: none
		* EXPORTS: unordered_map of entitys in world.
		* PURPOSE: To retrieve a map of the systems in the world. Type is the key.
		*/
		std::unordered_map<std::type_index, std::shared_ptr<System>>& GetSystemDatabase();

	private:
		std::unordered_map<std::string, std::shared_ptr<Entity>> m_entityDatabase;
		std::unordered_map<std::string, std::shared_ptr<Entity>> m_removedEntitys;

		std::unordered_map<std::type_index, std::shared_ptr<System>> m_systemDatabase;
	};

	template<typename T>
	void World::AddSystem(std::unique_ptr<System> system)
	{
		if (m_systemDatabase.find(std::type_index(typeid(T))) != m_systemDatabase.end())
		{
			RE_LOG(LogLevel::WARNING, "Tried to create a pre existing system");
		}
		else
		{
			m_systemDatabase[typeid(T)] = std::move(system);
		}
	}

	template<typename T>
	void World::DeleteSystem()
	{
		if (m_systemDatabase.find(std::type_index(typeid(T))) != m_systemDatabase.end())
		{
			m_systemDatabase.erase(typeid(T));
		}
		else
		{
			RE_LOG(LogLevel::WARNING, "Tried to remove a non-existant system");
		}
	}

	template<typename T>
	std::shared_ptr<T> World::GetSystem()
	{
		auto it = m_systemDatabase.find(std::type_index(typeid(T)));

		if (it != m_systemDatabase.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		else
		{
			return nullptr;
		}
	}
}

#endif