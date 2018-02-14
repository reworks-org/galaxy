///
/// World.cpp
/// starlight
///
/// Created by reworks on 09/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <map>

#include "sol2/sol.hpp"
#include "sl/fs/VFS.hpp"
#include "loguru/loguru.hpp"

#include "World.hpp"

namespace sl
{
	World::World()
	{
		m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::utf8);
	}

	void World::createEntity(const std::string& script)
	{
		sol::state lua;
		lua.script(VFS::inst()->openAsString(script));
		
		entt::Entity entity = m_registry.create();
		sol::table components = lua.get<sol::table>("entity");

		std::map<entt::HashedString::hash_type, sol::table> kvp;
		components.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			kvp.insert({ pair.first.as<entt::HashedString>(), pair.second.as<sol::table>() });
		});

		if (components.get<bool>("hasTags"))
		{
			sol::table tags = components.get<sol::table>("tags");

			std::map<int, entt::HashedString::hash_type> tagsKVP;
			tags.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				tagsKVP.insert({ pair.first.as<int>(), pair.second.as<entt::HashedString>() });
			});

			for (auto& it : tagsKVP)
			{
				m_tagAssign[it.second](entity);
			}

			kvp.erase(entt::HashedString{ "hasTags" });
		}

		kvp.erase(entt::HashedString{ "hasTags" });

		for (auto& it : kvp)
		{
			m_componentAssign[it.first](entity, entity, it.second);
		}

		m_inUse.push_back(entity);
	}

	void World::createEntities(const std::string& batchScript)
	{	
		sol::state lua;
		lua.script(VFS::inst()->openAsString(batchScript));

		sol::table world = lua.get<sol::table>("world");
		sol::table entityList = world.get<sol::table>("entitys");
		entityList.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			createEntity(pair.second.as<std::string>());
		});
	}

	void World::event(ALLEGRO_EVENT* event)
	{
		for (auto& system : m_systems)
		{
			system.second->event(event, m_registry);
		}
	}

	void World::update(const double dt)
	{
		m_currentLevel->update(dt);

		for (auto& system : m_systems)
		{
			system.second->update(dt, m_registry);
		}
	}

	void World::destroyCurrentEntities()
	{
		for (auto& entity : m_inUse)
		{
			m_registry.destroy(entity);
		}
	}
}