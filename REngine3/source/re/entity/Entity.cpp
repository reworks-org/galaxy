//
//  Entity.cpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/scripting/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/app/World.hpp"

namespace re
{
	Entity::Entity()
	{
	}

	void Entity::Init(const std::string& script, ComponentHolder& cl)
	{
		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		// Get a table with the components.
		sol::table entity = lua.get<sol::table>("entity");

		m_name = entity.get<std::string>("name");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		entity.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		// Create list.
		cl[m_name] = ComponentList();

		for (auto& kvp : m_keyValuePair)
		{
			auto cf = Locator::Get<World>()->GetComponentFactory().find(kvp.first);
			auto end = Locator::Get<World>()->GetComponentFactory().end();

			if (cf != end)
			{
				cl[m_name][cf->second.first] = cf->second.second();
				cl[m_name][cf->second.first]->Init(kvp.second);
			}
		}

		m_components = &cl[m_name];
	}

	Entity::~Entity()
	{
		m_systemIds.clear();
	}
}