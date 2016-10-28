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
	Entity::Entity(const std::string& script)
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

		for (auto& kvp : m_keyValuePair)
		{
			auto cf = Locator::Get<World>()->GetComponentFactory().find(kvp.first);
			auto end = Locator::Get<World>()->GetComponentFactory().end();

			if (cf != end)
			{
				m_components[cf->second.first] = cf->second.second();
				m_components[cf->second.first]->Init(kvp.second);
			}
		}
	}

	Entity::~Entity()
	{
		m_systemIds.clear();
		m_components.clear();
	}
}