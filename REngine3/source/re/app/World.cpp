//
//  World.cpp
//  REngine3
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/component/TextComponent.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"
#include "re/component/EventComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/AnimatedSpriteComponent.hpp"
#include "re/systems/RenderSystem.hpp"
#include "World.hpp"

namespace re
{
	World::World()
	{
		AddCustomComponent<TextComponent>("TextComponent");
		AddCustomComponent<SoundComponent>("SoundComponent");
		AddCustomComponent<MusicComponent>("MusicComponent");
		AddCustomComponent<EventComponent>("EventComponent");
		AddCustomComponent<SpriteComponent>("SpriteComponent");
		AddCustomComponent<AnimatedSpriteComponent>("AnimatedSpriteComponent");
	}

	World::~World()
	{
		m_dead.clear();
		m_alive.clear();
		m_systems.clear();
		m_entityComponentList.clear();
		m_componentFactory.clear();
	}

	void World::Register(const std::string& entitysScript)
	{
		sf::Uint64 counter = 0;

		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(entitysScript));
		sol::table world = lua.get<sol::table>("world");

		m_alive.resize(0);
		m_dead.resize(0);

		int max = lua.get<int>("numEntitys");

		m_alive.reserve(max);
		m_dead.reserve(max);

		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <std::string, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		for (auto& it : m_keyValuePair)
		{
			m_alive.push_back(Entity(it.second, counter, this));
			counter++;
		}
	}

	void World::Update(sf::Time dt)
	{
		for (auto& v : m_alive)
		{
			// TODO: Change this to copy a reference over copying the object.
			if (v.m_isDead)
			{
				for (auto s : v.m_systemIds)
				{
					m_systems[s]->RemoveEntity(v.m_id);
				}

				m_dead.push_back(v);
				m_alive.erase(std::find(m_alive.begin(), m_alive.end(), v));
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

				m_alive.push_back(v);
				m_dead.erase(std::find(m_dead.begin(), m_dead.end(), v));
			}
		}
	}

	std::unordered_map<sf::Uint64, ComponentList>* World::GetComponentList()
	{
		return &m_entityComponentList;
	}

	ComponentFactory* World::GetComponentFactory()
	{
		return &m_componentFactory;
	}
}