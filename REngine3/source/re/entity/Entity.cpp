//
//  Entity.cpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/app/World.hpp"

#include "EntityManager.hpp"

namespace re
{
	Entity::Entity(const std::string& script, sf::Uint64 id, World* world)
	{
		m_world = world;
		m_id = id;

		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		// Get a table with the components.
		sol::table entity = lua.get<sol::table>("entity");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		entity.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		ComponentList temp;
		for (auto& it : m_keyValuePair)
		{
			auto c = m_world->GetComponentFactory()->find(it.first);
			temp.emplace(c->second.first, c->second.second());
		}

		m_world->GetComponentList()->emplace(m_id, temp);
		m_components = &m_world->GetComponentList()->at(m_id);
		
		Locator::Get<EntityManager>()->Add(entity.get<std::string>("name"), this);
	}

	Entity::~Entity()
	{
		m_systemIds.clear();
		m_components->clear();
	}
}