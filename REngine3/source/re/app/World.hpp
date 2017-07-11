//
//  World.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WORLD_HPP_
#define RENGINE3_WORLD_HPP_

#include "re/utility/Log.hpp"
#include "re/types/System.hpp"
#include "re/types/Service.hpp"
#include "re/types/Component.hpp"

namespace sf
{
	class Time;
}

namespace re
{
	typedef std::map<std::string, Entity> EntityList;
	typedef std::unordered_map<std::type_index, std::pair<std::string, std::shared_ptr<System>>> SystemList;
	typedef std::unordered_map<std::string, std::pair<std::type_index, std::function<std::shared_ptr<Component>()>>> ComponentFactory;

	class World : public Service
	{
		friend class Entity;

	public:       
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up world.
		*/
		~World() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Set up world.
		*/
		void init();

		/*
		* IMPORTS: worldEntityScript - script file containing all the entitys to mass register.
		* EXPORTS: none
		* PURPOSE: To create new entitys and add them to the world.
		*/
		void registerEntitys(const std::string& worldEntityScript);

		/*
		* IMPORTS: entityScript - script file of an entity
		* EXPORTS: none
		* PURPOSE: To create a new entity and add it to the world.
		*/
		void registerEntity(const std::string& entityName, const std::string& entityScript);

		/*
		* IMPORTS: name of an entity to get
		* EXPORTS: reference to that entity
		* PURPOSE: To retrieve an entity from the world.
		*/
		Entity& getEntity(const std::string& name);

		/*
		* IMPORTS: name - name of entity to kill.
		* EXPORTS: none
		* PURPOSE: Remove entity from systems, "killing" it.
		*/
		void killEntity(const std::string& name);

		/*
		* IMPORTS: name - name of entity to revive.
		* EXPORTS: none
		* PURPOSE: Add entity back to systems, "restoring (reviving)" it.
		*/
		void restoreEntity(const std::string& name);

		/*
		* IMPORTS: sf::Time delta time
		* EXPORTS: none
		* PURPOSE: Update world & entitys.
		*/
		void update(sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans the world.
		*/
		void clean();

		/*
		* IMPORTS: type of component and name of component to register.
		REMEMBER TO USE 'new' and the correct system type!
		* EXPORTS: none
		* PURPOSE: To create a new component within the factory.
		*/
		template<typename T>
		void registerComponent(const std::string& name);

		/*
		* IMPORTS: s - The system to create. This uses polymorphism. Define the type of system being created with the template.
		REMEMBER TO USE 'new' and the correct system type!
		* EXPORTS: none
		* PURPOSE: To create a system that operates on the entities and add it to the world. You can only have one of each system.
		*/
		template<typename T>
		void addSystem(std::shared_ptr<System> system);

		/*
		* IMPORTS: template - use the type of system to retrieve that system.
		* EXPORTS: shared_ptr to that system.
		* PURPOSE: To retrieve a system from the world.
		*/
		template<typename T>
		std::shared_ptr<T> getSystem();

		/*
		* IMPORTS: none
		* EXPORTS: map of component factory.
		* PURPOSE: To "make" a new component.
		*/
		ComponentFactory& getComponentFactory();

		/*
		* IMPORTS: none
		* EXPORTS: map of alive entitys.
		* PURPOSE: To get the alive entity database.
		*/
		EntityList& getAliveEntitys();

		/*
		* IMPORTS: none
		* EXPORTS: map of dead entitys.
		* PURPOSE: To get the dead entity database.
		*/
        EntityList& getDeadEntitys();
		
		/*
		* IMPORTS: none
		* EXPORTS: map of systems.
		* PURPOSE: To get the map of systems.
		*/
		SystemList& getSystemList();

        /*
         * IMPORTS: none
         * EXPORTS: none
         * PURPOSE: Set boolean flag if entitys have been changed.
         */
        void entitysHaveChanged();
        
	private:
		/*
		* IMPORTS: type of component to make
		* EXPORTS: shared_ptr to component.
		* PURPOSE: To make a component of a particular type. Used by register component.
		*/
		template<typename T>
		std::shared_ptr<Component> makeComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Adds entitys to correct systems.
		*/
		void emplaceEntitysInSystems();

	private:
		EntityList m_dead;
		EntityList m_alive;
		SystemList m_systems;
		ComponentHolder m_components;
		ComponentFactory m_componentFactory;
        
		bool m_entitysHaveChanged = false;
        
    public:
        std::vector<std::string> m_loadedEntityScripts;
        std::unordered_map<std::string, std::type_index> m_stringToComponentType;
	};

	template<typename T>
	void World::addSystem(std::shared_ptr<System> system)
	{
        if(m_systems.find(std::type_index(typeid(T))) != m_systems.end())
        {
            RE_LOG(LogLevel::WARNING, "Tried to create a pre existing system", "World::addSystem", "World.hpp", 156);
        }
        else
        {
            m_systems[typeid(T)] = std::make_pair(system->getTypeAsString(), system);
        }
	}

	template<typename T>
	std::shared_ptr<T> World::getSystem()
	{
        auto it = m_systems.find(std::type_index(typeid(T)));
        
        RE_REVERSE_ASSERT_COMPARE(it, m_systems.end(), "Tried to access a non-existent system", "World::getSystem", "World.cpp", 169);
        
        return std::dynamic_pointer_cast<T>(it->second.second);
	}

	template<typename T>
	std::shared_ptr<Component> World::makeComponent()
	{
		return std::make_shared<T>();
	}

	template<typename T>
	void World::registerComponent(const std::string& name)
	{
		RE_LOG(LogLevel::INFO, "Registering " + name + " in world", "World::registerComponent", "World.hpp", 183);
		m_componentFactory.emplace(name, std::pair<std::type_index, std::function<std::shared_ptr<Component>()>>(std::type_index(typeid(T)), std::bind(&World::makeComponent<T>, this)));
        m_stringToComponentType.emplace(name, std::type_index(typeid(T)));
	}
}

#endif
