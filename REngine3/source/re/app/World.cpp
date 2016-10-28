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
#include "re/component/TimeComponent.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"
#include "re/component/EventComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/PositionComponent.hpp"
#include "re/component/AnimatedSpriteComponent.hpp"

#include "World.hpp"

namespace re
{
	World::World()
	{
		RegisterComponent<TextComponent>("TextComponent");
		RegisterComponent<TimeComponent>("TimeComponent");
		RegisterComponent<SoundComponent>("SoundComponent");
		RegisterComponent<MusicComponent>("MusicComponent");
		RegisterComponent<EventComponent>("EventComponent");
		RegisterComponent<SpriteComponent>("SpriteComponent");
		RegisterComponent<PositionComponent>("PositionComponent");
		RegisterComponent<AnimatedSpriteComponent>("AnimatedSpriteComponent");
	}

	World::~World()
	{
		m_dead.clear();
		m_alive.clear();
		m_systems.clear();
		m_componentFactory.clear();
	}

	void World::Register(const std::string& entitysScript)
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(entitysScript));
		sol::table world = lua.get<sol::table>("world");

		int max = world.get<int>("numEntitys");

		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <std::string, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		for (auto& it : m_keyValuePair)
		{
			std::shared_ptr<Entity> e = std::make_shared<Entity>(it.second);
			m_alive.emplace(e->m_name, e);
		}
	}

	void World::KillEntity(const std::string& name)
	{
		if (m_alive.find(name) != m_alive.end())
		{
			m_alive[name]->m_isDead = true;
		}
	}

	void World::ReviveEntity(const std::string& name)
	{
		if (m_dead.find(name) != m_dead.end())
		{
			m_dead[name]->m_isDead = false;
		}
	}

	std::shared_ptr<Entity> World::Get(const std::string& name)
	{
		return m_alive[name];
	}

	void World::Update(sf::Time dt)
	{
		for (auto& it : m_alive)
		{
			if (it.second->m_isDead)
			{
				std::string name = it.second->m_name;
				for (auto s : it.second->m_systemIds)
				{
					m_systems[s.second]->RemoveEntity(name);
				}

				m_dead.emplace(name, it.second);
				m_alive.erase(name);
			}
		}

		for (auto& it : m_dead)
		{
			if (!it.second->m_isDead)
			{
				std::string name = it.second->m_name;
				for (auto s : it.second->m_systemIds)
				{
					m_systems[s.second]->AddEntity(it.second);
				}

				m_alive.emplace(name, it.second);
				m_dead.erase(name);
			}
		}

		for (auto& it : m_alive)
		{
			for (auto& c : it.second->m_components)
			{
				c.second->Update(dt);
			}
		}
	}

	void World::Clean()
	{
		m_alive.clear();
		m_dead.clear();
	}

	ComponentFactory& World::GetComponentFactory()
	{
		return m_componentFactory;
	}

	EntityDatabase& World::GetAlive()
	{
		return m_alive;
	}

	EntityDatabase& World::GetDead()
	{
		return m_dead;
	}
}