///
///  World.cpp
///  rework
///
///  Created by reworks on 09/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <map>

#include "sol2/sol.hpp"
#include "loguru/loguru.hpp"
#include "re/services/VFS.hpp"

#include "World.hpp"

namespace re
{
	void World::createEntity(std::string_view script)
	{
		sol::state lua;
		lua.script(VFS::get()->openAsString(script));
		
		entt::Entity entity = m_registery.create();
		sol::table components = lua.get<sol::table>("entity");

		std::map<std::string, sol::table> kvp;
		components.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			kvp.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		if (components.get<bool>("hasTags"))
		{
			sol::table tags = componets.get<sol::table>("tags");

			std::map<int, std::string> tagsKVP;
			components.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				tagsKVP.insert({ pair.first.as<int>(), pair.second.as<std::string>() });
			});

			for (auto& it : tagsKVP)
			{
				m_tagAssign[it.second](entity);
			}

			kvp.erase("tags");
		}
		kvp.erase("hasTags");

		for (auto& it : kvp)
		{
			m_componentAssign[it.first](entity, it.second);
		}
	}

	void World::createEntities(std::string_view batchScript)
	{	
		std::map<int, std::string_view> kvp;

		{
			sol::state lua;
			lua.script(VFS::get()->openAsString(batchScript));

			sol::table world = lua.get<sol::table>("world");
			sol::table entityList = world.get<sol::table>("entitys");

			entitylist.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				kvp.insert({ pair.first.as<int>(), pair.second.as<std::string_view>() });
			});
		}

		for (auto& it : kvp)
		{
			createEntity(it.second);
		}
	}

	void World::update(const double dt)
	{
		for (auto& system : m_systems)
		{
			system.second->update(dt, m_registery);
		}
	}
}