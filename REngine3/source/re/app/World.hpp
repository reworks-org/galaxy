//
//  World.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WORLD_HPP_
#define RENGINE3_WORLD_HPP_

#include "re/types/System.hpp"
#include "re/types/Service.hpp"

namespace sf
{
	class Time;
}

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
		void Register(const std::string& entitysScript);

		/*
		* IMPORTS: sf::Time delta time
		* EXPORTS: none
		* PURPOSE: Update world & entitys.
		*/
		void Update(sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans the world.
		*/
		void Clean();

		/*
		* IMPORTS: s - The system to create. This uses polymorphism. Define the type of system being created with the template.
		REMEMBER TO USE 'new' and the correct system type!
		* EXPORTS: none
		* PURPOSE: To create a system that operates on the entities and add it to the world. You can only have one of each system.
		*/
		template<typename T>
		void AddSystem(std::shared_ptr<System> system);

		/*
		* IMPORTS: template - use the type of system to retrieve that system.
		* EXPORTS: shared_ptr to that system.
		* PURPOSE: To retrieve a system from the world.
		*/
		template<typename T>
		std::shared_ptr<T> Get();

	private:
		std::vector<Entity> m_alive;
		std::vector<Entity> m_dead;
		std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;
	};

	template<typename T>
	void World::AddSystem(std::shared_ptr<System> system)
	{
		if (m_systems.find(std::type_index(typeid(T))) != m_systems.end())
		{
			RE_LOG(LogLevel::WARNING, "Tried to create a pre existing system");
		}
		else
		{
			m_systems[typeid(T)] = system;
		}
	}

	template<typename T>
	std::shared_ptr<T> World::Get()
	{
		return std::dynamic_pointer_cast<T>(m_systems.find(std::type_index(typeid(T)))->second);
	}
}

#endif