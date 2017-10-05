//
//  World.hpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_WORLD_HPP_
#define RENGINE3_WORLD_HPP_

#include <allegro5/timer.h>

#include "re/types/Service.hpp"

namespace re
{
	typedef std::map<std::string, Entity> EntityList;
	typedef std::unordered_map<std::type_index, std::pair<std::string, std::unique_ptr<System>>> SystemList;
	typedef std::unordered_map<std::string, std::pair<std::type_index, std::function<std::unique_ptr<Component>(sol::table&)>>> ComponentFactory;

	class World : public Service
	{
		friend class Entity;
	public: 
		///
		/// \brief Construct World.
		///
		/// This also registers the inbuilt components.
		///
		World();

		///
		/// Cleans up world.
		///
		~World() override;

		///
		/// Register an entity.
		///
		/// \param name Entity Name.
		/// \param script Script file in the VFS.
		///
		void registerEntity(const std::string& name, const std::string& script);

		///
		/// Create a batch of entitys and add then to the world.
		///
		/// \param worldEntityScript Script file containing all the entitys to mass register.
		///
		void registerEntitys(const std::string& worldEntityScript);

		///
		/// To create entitys that are delayed from being put into the systems, but already have their components loaded.
		///
		/// \param preloadEntitysScript Script file containing all the entitys to mass preload.
		///
		void preloadEntitys(const std::string& preloadEntitysScript);

		///
		/// \brief Moves loaded entitys to alive/dead storage.
		///
		/// Specifically, moves entitys from m_preloaded to m_alive/m_dead.
		///
		void activatePreloadedEntitys();

		///
		/// Retrieve an entity from the world.
		///
		/// \param name The name of the entity to retrieve.
		///
		/// \return Returns a reference to the entity.
		///
		Entity& get(const std::string& name);

		///
		/// \brief Kill an entity.
		///
		/// Removes an entity from m_alive and moves it to m_dead.
		///
		/// \param name Name of entity to kill.
		///
		void killEntity(const std::string& name);

		///
		/// \brief Revive an entity.
		///
		/// Removes an entity from m_dead and moves it to m_alive.
		///
		/// \param name Name of entity to revive.
		///
		void reviveEntity(const std::string& name);

		///
		/// \brief Update the world.
		///
		/// 
		void update(ALLEGRO_TIMER* dt);

		///
		/// \brief Clear entitys from the world.
		///
		/// This also removes any component data assositated with those entitys.
		///
		void clearEntitys();

		///
		/// Cleans the preloaded entitys from world.
		///
		void clearPreloaded();

		///
		/// Retrieve internal component factory.
		///
		/// \return Reference to component factory.
		///
		ComponentFactory& getComponentFactory();

		///
		/// Retrieve internal alive entitys list.
		///
		/// \return Reference to alive entitys list.
		///
		EntityList& getAliveEntitys();

		///
		/// Retrieve internal dead entitys list.
		///
		/// \return Reference to dead entitys list.
		///
        EntityList& getDeadEntitys();
		
		///
		/// Retrieve internal system list.
		///
		/// \return Reference to system list.
		///
		SystemList& getSystemList();
		
		///
		/// \brief Set boolean flag if entitys have been changed.
		///
		/// This triggers the update entitys code in the update function.
		///
        void entitysHaveChanged();

		///
		/// Registers a new component with the world.
		///
		/// \param template Type of component to register.
		/// \param name The name of the component (Should be same name as class/type).
		///
		template<typename T>
		void registerComponent(const std::string& name);

		///
		/// Registers a new system with the world.
		///
		/// \param template Type of the system to register.
		/// \param system A unique_ptr to the new system. Use std::make_unique<T>()
		///
		template<typename T>
		void registerSystem(std::unique_ptr<System> system);

		///
		/// To retrieve a system from the world.
		///
		/// \param  template Use the type of system to retrieve that system.
		///
		/// \return Pointer to that system.
		///
		template<typename T>
		T* get();
        
	private:
		///
		/// Internal function to create components in factory.
		///
		template<typename T>
		std::unique_ptr<Component> makeComponent(sol::table& table);

		///
		/// Internal function to emplace entitys into correct systems.
		///
		void emplaceEntitysInSystems();

	private:
		EntityList m_dead;
		EntityList m_alive;
		EntityList m_preloaded;
		SystemList m_systems;
		ComponentHolder m_components;
		ComponentFactory m_componentFactory;
        
		bool m_entitysHaveChanged;
        
    public:
        std::vector<std::string> m_loadedEntityScripts;
		std::vector<std::string> m_preloadedEntityScripts;
        std::unordered_map<std::string, std::type_index> m_stringToComponentType;
	};

	template<typename T>
	void World::registerSystem(std::unique_ptr<System> system)
	{
        if(m_systems.find(std::type_index(typeid(T))) != m_systems.end())
        {
            RE_LOG(LogLevel::WARNING, "Tried to create a pre existing system", "World::registerSystem", "World.hpp", 209);
        }
        else
        {
            m_systems[typeid(T)] = std::make_pair(system->getTypeAsString(), std::move(system));
        }
	}

	template<typename T>
	T* World::get()
	{
		if (m_systems.find(std::type_index(typeid(T))) != m_systems.end())
		{
			return dynamic_cast<T*>(it->second.second.get());
		}
		else
		{
			RE_LOG(LogLevel::Warning, "Tried to access a non-existent system.", "World::get<System>", "World.hpp", 226);
			return nullptr;
		}
	}

	template<typename T>
	std::unique_ptr<Component> World::makeComponent(sol::table& table)
	{
		return std::move(std::make_unique<T>(table));
	}

	template<typename T>
	void World::registerComponent(const std::string& name)
	{
		RE_LOG(LogLevel::INFO, "Registering " + name + " in world", "World::registerComponent", "World.hpp", 240);
		m_componentFactory.emplace(name, std::make_pair(std::type_index(typeid(T)), std::bind(&World::makeComponent<T>, this, std::placeholders::_1)));
        m_stringToComponentType.emplace(name, std::type_index(typeid(T)));
	}
}

#endif