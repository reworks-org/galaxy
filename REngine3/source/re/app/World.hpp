//
//  World.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WORLD_HPP_
#define RENGINE3_WORLD_HPP_

#include "re/types/Types.hpp"
#include "re/types/System.hpp"
#include "re/types/Service.hpp"
#include "re/entity/Entity.hpp"

namespace re
{
	typedef std::unordered_map<std::string, std::pair<std::type_index, std::function<std::shared_ptr<Component>()>>> ComponentFactory;
	typedef std::unordered_map<std::type_index, std::shared_ptr<System>> SystemList;

	class World : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Construct world.
		*/
		World();

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
		* EXPORTS: pointer to component list
		* PURPOSE: Get and Set components for entitys.
		*/
		std::unordered_map<sf::Uint64, ComponentList>* GetComponentList();

		/*
		* IMPORTS: none
		* EXPORTS: pointer to component list
		* PURPOSE: get component factory.
		*/
		ComponentFactory* GetComponentFactory();

		/*
		* IMPORTS: std::string of the name of the component.
		* EXPORTS: none
		* PURPOSE: To create a custom component the engine doesn't have.
		*/
		template<typename T>
		void AddCustomComponent(const std::string& name);

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

		std::unordered_map<sf::Uint64, ComponentList> m_entityComponentList;
		SystemList m_systems;
		ComponentFactory m_componentFactory;
	};

	template<typename T>
	void World::AddCustomComponent(const std::string& name)
	{
		m_componentFactory[name] = std::make_pair(typeid(T),
			[this]()
			{
				return std::make_shared<T>();
			};
		);	
	}
	
	template<typename T>
	void World::AddSystem(std::shared_ptr<System> system)
	{
		if (m_systemDatabase.find(std::type_index(typeid(T))) != m_systemDatabase.end())
		{
			RE_LOG(LogLevel::WARNING, "Tried to create a pre existing system");
		}
		else
		{
			m_systemDatabase[typeid(T)] = system;
		}
	}

	template<typename T>
	std::shared_ptr<T> World::Get()
	{
		return std::dynamic_pointer_cast<T>(m_systems.find(std::type_index(typeid(T)))->second);
	}
}

#endif