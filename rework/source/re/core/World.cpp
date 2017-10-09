//
//  World.cpp
//  rework
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <map>

#include "sol2/sol.hpp"
#include "re/utility/Log.hpp"
#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

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
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(script));
		sol::table components = lua.get<sol::table>("entity");
		
		ex::Entity e = m_entityManager.create();

		// Get key-value pairs from table
		std::map <std::string, sol::table> m_keyValuePair;
		components.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		if (components.get<bool>("special"))
		{
			m_entitys.emplace(components.get<std::string>("name"), e);
			m_keyValuePair.erase("name");
		}
		
		m_keyValuePair.erase("special");

		for (auto& it : m_keyValuePair)
		{
			if (!assignSystemComponents(it.first, it.second, e))
			{
				if (!m_assignUserComponents(it.first, it.second, e))
				{
					BOOST_LOG_TRIVIAL(warning) << "Attempted to register unknown component! script: " << script << std::endl;
				}
			}
		}
	}

	void World::createEntities(const std::string& batchScript)
	{
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(batchScript));

		sol::table world = lua.get<sol::table>("world");
		sol::table entitylist = world.get<sol::table>("entitys");

		// Get key-value pairs from table
		std::map <int, std::string> m_keyValuePair;
		entitylist.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<int>(), pair.second.as<std::string>() });
		});

		if (m_keyValuePair.empty())
		{
			BOOST_LOG_TRIVIAL(error) << "Attempted to register an empty entity script." << std::endl;
			return;
		}

		for (auto& it : m_keyValuePair)
		{
			lua.script(Locator::get<VFS>()->openAsString(it.second));
			sol::table components = lua.get<sol::table>("entity");

			ex::Entity e = m_entityManager.create();

			// Get key-value pairs from table
			std::map <std::string, sol::table> m_ekv;
			components.for_each([&](std::pair<sol::object, sol::object> pair) {
				m_ekv.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
			});

			if (components.get<bool>("special"))
			{
				m_entitys.emplace(components.get<std::string>("name"), e);
				m_ekv.erase("name");
			}

			m_ekv.erase("special");

			for (auto& eit : m_ekv)
			{
				if (!assignSystemComponents(eit.first, eit.second, e))
				{
					if (!m_assignUserComponents(eit.first, eit.second, e))
					{
						BOOST_LOG_TRIVIAL(warning) << "Attempted to register unknown component! script: " << it.second << std::endl;
					}
				}
			}
		}
	}

	void World::update(double dt)
	{
		m_systemManager.update_all(dt);
	}

	void World::registerAssignUserComponentsFunction(std::function<bool(const std::string&, sol::table&, ex::Entity&)>& aucf)
	{
		m_assignUserComponents = aucf;
	}

	bool World::assignSystemComponents(const std::string& name, sol::table& table, ex::Entity& e)
	{
		bool out = true;

		if (name == "AnimationComponent")
		{
			e.assign<AnimationComponent>(table);
		}
		else if (name == "MusicComponent")
		{
			e.assign<MusicComponent>(table);
		}
		else if (name == "ParallaxComponent")
		{
			e.assign<ParallaxComponent>(table);
		}
		else if (name == "PhysicsComponent")
		{
			e.assign<PhysicsComponent>(table);
		}
		else if (name == "SoundComponent")
		{
			e.assign<SoundComponent>(table);
		}
		else if (name == "SpriteComponent")
		{
			e.assign<SpriteComponent>(table);
		}
		else if (name == "TextComponent")
		{
			e.assign<TextComponent>(table);
		}
		else if (name == "TransformComponent")
		{
			e.assign<TransformComponent>(table);
		}
		else
		{
			out = false;
		}

		return out;
	}
}