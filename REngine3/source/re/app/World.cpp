//
//  World.cpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include <SFML/System/Time.hpp>

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/entity/EntityManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "World.hpp"

namespace re
{
	World::~World()
	{
		m_dead.clear();
		m_alive.clear();
		m_systems.clear();
	}

	void World::Register(const std::string& entitysScript)
	{
		sf::Uint64 counter = 0;

		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(entitysScript));
		sol::table world = lua.get<sol::table>("world");

		int max = world.get<int>("numEntitys");

		m_alive.reserve(max + 1);
		m_dead.reserve(max + 1);

		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <std::string, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		for (auto& it : m_keyValuePair)
		{
			m_alive.push_back(Entity(it.second, counter));
			Locator::Get<EntityManager>()->Add(m_alive[counter].m_name, &m_alive[counter]);
			counter++;
		}
	}

	void World::Update(sf::Time dt)
	{
		for (auto& v : m_alive)
		{
			if (v.m_isDead)
			{
				for (auto s : v.m_systemIds)
				{
					m_systems[s]->RemoveEntity(v.m_id);
				}

				m_dead.insert(m_dead.begin() + v.m_id, v);
				m_alive.erase(m_alive.begin() + v.m_id);
			}
		}

		for (auto& v : m_dead)
		{
			if (!v.m_isDead)
			{
				for (auto s : v.m_systemIds)
				{
					m_systems[s]->AddEntity(&v);
				}

				m_alive.insert(m_alive.begin() + v.m_id, v);
				m_dead.erase(m_dead.begin() + v.m_id);
			}
		}
	}
}