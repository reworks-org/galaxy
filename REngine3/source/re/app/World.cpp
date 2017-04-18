//
//  World.cpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include <SFML/System/Time.hpp>

#include "re/services/vfs/VFS.hpp"
#include "re/scripting/sol2/sol.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/component/TextComponent.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"
#include "re/component/EventComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/PhysicsComponent.hpp"
#include "re/component/TransformComponent.hpp"
#include "re/component/AnimationComponent.hpp"

#include "World.hpp"

namespace re
{
	World::~World()
	{
		m_dead.clear();
		m_alive.clear();
		m_systems.clear();
		m_components.clear();
		m_componentFactory.clear();
	}

	void World::init()
	{
		registerComponent<TextComponent>("TextComponent");
		registerComponent<SoundComponent>("SoundComponent");
		registerComponent<MusicComponent>("MusicComponent");
		registerComponent<EventComponent>("EventComponent");
		registerComponent<SpriteComponent>("SpriteComponent");
		registerComponent<PhysicsComponent>("PhysicsComponent");
		registerComponent<TransformComponent>("TransformComponent");
		registerComponent<AnimationComponent>("AnimationComponent");
	}

	void World::registerEntitys(const std::string& entitysScript)
	{
		sol::state lua;
		lua.script(Locator::get<VFS>()->toString(entitysScript));
		sol::table world = lua.get<sol::table>("world");
		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <std::string, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		RE_REVERSE_ASSERT(m_keyValuePair.empty(), "Attempted to register an empty entity script", "World::registerEntitys", "World.cpp", 64);

		for (auto& it : m_keyValuePair)
		{
			m_alive.emplace(it.first, Entity());
			m_alive[it.first].init(it.second, m_components);
		}
	}

	Entity& World::getEntity(const std::string& name)
	{
		auto found = m_alive.find(name);
        
		RE_REVERSE_ASSERT_COMPARE(found, m_alive.end(), "Attempted to access non-existant entity", "World::getEntity", "World.cpp", 77);
        
        return m_alive[name];
	}

	void World::killEntity(const std::string& name)
	{
		if (m_alive.find(name) != m_alive.end())
		{
			m_alive[name].m_isDead = true;
		}
	}

	void World::restoreEntity(const std::string& name)
	{
		if (m_dead.find(name) != m_dead.end())
		{
			m_dead[name].m_isDead = false;
		}
	}

	void World::update(sf::Time dt)
	{
		for (auto it = m_alive.begin(); it != m_alive.end();)
		{
			if (it->second.m_isDead)
			{
				std::string name = it->second.m_name;
				for (auto& s : it->second.m_systemIds)
				{
					m_systems[s.second]->removeEntity(name);
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
                    m_systems[s.second]->addEntity(&m_alive[name]);
                }
			}
			else
			{
				++it;
			}
		}
	}

	void World::clean()
	{
		m_dead.clear();
		m_alive.clear();
		m_components.clear();
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
}
