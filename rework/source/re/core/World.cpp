//
//  World.cpp
//  rework
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "World.hpp"

// https://github.com/alecthomas/entityx/blob/master/README.md
// https://github.com/alecthomas/entityx/blob/master/examples/example.cc

namespace re
{
	World::World()
	:m_entityManager(m_eventManager), m_systemManager(m_entityManager, m_eventManager)
	{
	}

	World::~World()
	{
	}

	void World::createEntity(const std::string& script)
	{
		ex::Entity e = m_entityManager.create();
	}

	void World::createEntities(const std::string& batchScript)
	{
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(worldEntityScript));

		sol::table world = lua.get<sol::table>("world");
		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <std::string, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		if (m_keyValuePair.empty())
		{
			RE_LOG(LogLevel::WARNING, "Attempted to register an empty entity script", "World::registerEntitys", "World.cpp", 82);
		}

		for (auto& it : m_keyValuePair)
		{
			m_alive.emplace(it.first, Entity(it.second, m_components));
			m_loadedEntityScripts.push_back(it.second);
		}

		emplaceEntitysInSystems();
		m_entitysHaveChanged = true;
		
		update(nullptr);
	}

	Entity& World::get(const std::string& name)
	{
		if (m_alive.find(name) != m_alive.end())
		{
			return m_alive[name];
		}
		else
		{
			if (m_dead.find(name) != m_dead.end())
			{
				return m_dead[name];
			}
			else
			{
				// This will close program and prompt user with error.
				RE_LOG(LogLevel::FATAL, "Attempted to access non-existent entity", "World::get (entity)", "World.cpp", 159);

				// program never reaches this point, which is why we return an empty entity object.
				return Entity();
			}
		}
	}

	void World::killEntity(const std::string& name)
	{
		m_entitysHaveChanged = true;

		if (m_alive.find(name) != m_alive.end())
		{
			m_alive[name].m_isDead = true;
		}
	}

	void World::reviveEntity(const std::string& name)
	{
		m_entitysHaveChanged = true;

		if (m_dead.find(name) != m_dead.end())
		{
			m_dead[name].m_isDead = false;
		}
	}

	void World::update(ALLEGRO_TIMER* dt)
	{
		if (m_entitysHaveChanged)
		{
			for (auto it = m_alive.begin(); it != m_alive.end();)
			{
				if (it->second.m_isDead)
				{
					std::string name = it->second.m_name;
					for (auto& s : it->second.m_systemIds)
					{
						m_systems[s.second].second->removeEntity(name);
					}

					m_dead.insert(std::make_pair(name, std::move(it->second)));
					m_alive.erase(it++);
				}
				else
				{
					++it;
				}
			}

			for (auto it = m_dead.begin(); it != m_dead.end();)
			{
				if (!it->second.m_isDead)
				{
					std::string name = it->second.m_name;

					m_alive.insert(std::make_pair(name, std::move(it->second)));
					m_dead.erase(it++);

					for (auto& s : m_alive[name].m_systemIds)
					{
						m_systems[s.second].second->addEntity(&m_alive[name]);
					}
				}
				else
				{
					++it;
				}
			}

			m_entitysHaveChanged = false;
		}
	}
	
	void World::clearEntitys()
	{
		// remove alive and dead components, but not preloaded. they are cleared at the end.
		for (auto& it : m_alive)
		{
			m_components.erase(it.second.m_name);
		}

		for (auto& it : m_dead)
		{
			m_components.erase(it.second.m_name);
		}

		m_dead.clear();
		m_alive.clear();
		m_loadedEntityScripts.clear();
	}

	void World::clearPreloaded()
	{
		for (auto& it : m_preloaded)
		{
			m_components.erase(it.second.m_name);
		}

		m_preloaded.clear();
		m_preloadedEntityScripts.clear();
		
	}

	ComponentFactory& World::getComponentFactory()
	{
		return m_componentFactory;
	}

	EntityList& World::getAliveEntitys()
	{
		return m_alive;
	}

	EntityList& World::getDeadEntitys()
	{
		return m_dead;
	}

	SystemList& World::getSystemList()
	{
		return m_systems;
	}
    
    void World::entitysHaveChanged()
    {
        m_entitysHaveChanged = true;
    }

	void World::emplaceEntitysInSystems()
	{
		for (auto it = m_alive.begin(); it != m_alive.end(); ++it)
		{
			for (auto& s : it->second.m_systemIds)
			{
				m_systems[s.second].second->addEntity(&it->second);
			}
		}
	}
}
