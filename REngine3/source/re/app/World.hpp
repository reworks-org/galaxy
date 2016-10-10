//
//  World.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WORLD_HPP_
#define RENGINE3_WORLD_HPP_

#include <utility>
#include <functional>

#include "re/types/System.hpp"
#include "re/types/Service.hpp"

namespace sf
{
	class Time;
}

namespace re
{
	typedef std::unordered_map<std::type_index, std::shared_ptr<System>> SystemList;
	typedef std::unordered_map<std::string, std::pair<std::type_index, std::function<std::shared_ptr<Component>()>>> ComponentFactory;

	class World : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Set up world.
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
		* EXPORTS: none
		* PURPOSE: Cleans the world.
		*/
		void Clean();

		/*
		* IMPORTS: type of component and name of component to register.
		REMEMBER TO USE 'new' and the correct system type!
		* EXPORTS: none
		* PURPOSE: To create a new component within the factory.
		*/
		template<typename T>
		void RegisterComponent(const std::string& name);

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

		/*
		* IMPORTS: none
		* EXPORTS: map of component factory.
		* PURPOSE: To "make" a new component.
		*/
		ComponentFactory& GetComponentFactory();
		
	private:
		/*
		* IMPORTS: type of component to make
		* EXPORTS: shared_ptr to component.
		* PURPOSE: To make a component of a particular type. Used by register component.
		*/
		template<typename T>
		std::shared_ptr<Component> MakeComponent();

	private:
		std::vector<Entity> m_alive;
		std::vector<Entity> m_dead;
		SystemList m_systems;
		ComponentFactory m_componentFactory;
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

	template<typename T>
	std::shared_ptr<Component> World::MakeComponent()
	{
		return std::make_shared<T>();
	}

	template<typename T>
	void World::RegisterComponent(const std::string& name)
	{
		m_componentFactory.emplace(name, std::pair<std::type_index, std::function<std::shared_ptr<Component>()>>(std::type_index(typeid(T)), std::bind(&World::MakeComponent<T>, this)));
	}
}

#endif