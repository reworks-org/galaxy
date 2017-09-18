//
//  World.cpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include <SFML/System/Time.hpp>

#include "re/services/VFS.hpp"
#include "re/scripting/sol2/sol.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/component/TextComponent.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"
#include "re/component/EventComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/PhysicsComponent.hpp"
#include "re/component/ParallaxComponent.hpp"
#include "re/component/TransformComponent.hpp"
#include "re/component/AnimationComponent.hpp"
#include "re/component/CollisionComponent.hpp"

#include "World.hpp"

namespace re
{
	World::World()
	:m_entitysHaveChanged(false)
	{

	}

	World::~World()
	{
		m_dead.clear();
		m_alive.clear();
		m_preloaded.clear();
		m_systems.clear();
		m_components.clear();
		m_componentFactory.clear();
		m_loadedEntityScripts.clear();
		m_preloadedEntityScripts.clear();
	}

	void World::init()
	{
		registerComponent<TextComponent>("TextComponent");
		registerComponent<SoundComponent>("SoundComponent");
		registerComponent<MusicComponent>("MusicComponent");
		registerComponent<EventComponent>("EventComponent");
		registerComponent<SpriteComponent>("SpriteComponent");
		registerComponent<PhysicsComponent>("PhysicsComponent");
		registerComponent<ParallaxComponent>("ParallaxComponent");
		registerComponent<TransformComponent>("TransformComponent");
		registerComponent<CollisionComponent>("CollisionComponent");
		registerComponent<AnimationComponent>("AnimationComponent");
	}

	void World::registerEntity(const std::string& entityName, const std::string& entityScript)
	{
		m_alive.emplace(entityName, Entity());
		m_alive[entityName].init(entityScript, m_components);

		m_loadedEntityScripts.push_back(entityScript);

		emplaceEntitysInSystems();

		m_entitysHaveChanged = true;
		update(sf::Time::Zero);
	}

	void World::registerEntitys(const std::string& worldEntityScript)
	{
		sol::state lua;
		lua.script_file(Locator::get<VFS>()->retrieve(worldEntityScript));
		sol::table world = lua.get<sol::table>("world");
		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <std::string, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		RE_REVERSE_ASSERT(m_keyValuePair.empty(), "Attempted to register an empty entity script", "World::registerEntitys", "World.cpp", 78);

		for (auto& it : m_keyValuePair)
		{
			m_alive.emplace(it.first, Entity());
			m_alive[it.first].init(it.second, m_components);

			m_loadedEntityScripts.push_back(it.second);
		}

		emplaceEntitysInSystems();

		m_entitysHaveChanged = true;
		update(sf::Time::Zero);
	}

	void World::preloadEntitys(const std::string& preloadEntitysScript)
	{
		sol::state lua;
		lua.script_file(Locator::get<VFS>()->retrieve(preloadEntitysScript));
		sol::table world = lua.get<sol::table>("world");
		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <std::string, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		RE_REVERSE_ASSERT(m_keyValuePair.empty(), "Attempted to register an empty entity script", "World::preloadEntitys", "World.cpp", 107);

		for (auto& it : m_keyValuePair)
		{
			m_preloaded.emplace(it.first, Entity());
			m_preloaded[it.first].init(it.second, m_components);

			m_preloadedEntityScripts.push_back(it.second);
		}
	}

	void World::activatePreloadedEntitys()
	{
		for (auto it = m_preloaded.begin(); it != m_preloaded.end();)
		{
			m_alive.insert(std::make_pair(it->second.m_name, std::move(it->second)));
			m_preloaded.erase(it++);
		}

		emplaceEntitysInSystems();

		m_entitysHaveChanged = true;
		update(sf::Time::Zero);

		m_preloaded.clear();
		
		for (auto& v : m_preloadedEntityScripts)
		{
			m_loadedEntityScripts.push_back(v);
		}

		m_preloadedEntityScripts.clear();
	}

	Entity& World::getEntity(const std::string& name)
	{
		auto found = m_alive.find(name);

		if (found != m_alive.end())
		{
			return m_alive[name];
		}
		else
		{
			auto it = m_dead.find(name);

			RE_REVERSE_ASSERT_COMPARE(it, m_dead.end(), "Attempted to access non-existant entity", "World::getEntity", "World.cpp", 130);

			return m_dead[name];
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

	void World::restoreEntity(const std::string& name)
	{
		m_entitysHaveChanged = true;

		if (m_dead.find(name) != m_dead.end())
		{
			m_dead[name].m_isDead = false;
		}
	}

	void World::update(sf::Time dt)
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
	
	void World::clean()
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

	void World::cleanPreloaded()
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
}
