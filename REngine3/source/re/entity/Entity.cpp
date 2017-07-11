//
//  Entity.cpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/services/VFS.hpp"
#include "re/scripting/sol2/sol.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/app/World.hpp"

namespace re
{
	Entity::Entity()
	{
        m_name = "";
        m_components = nullptr;
        m_systemIds.clear();
        m_isDead = false;
	}

	Entity::Entity(Entity&& e)
	{
		m_name = e.m_name;
		m_components = e.m_components;
		m_systemIds = e.m_systemIds;
		m_isDead = e.m_isDead;
	}
    
	void Entity::init(const std::string& script, ComponentHolder& cl)
	{
		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script_file(Locator::get<VFS>()->retrieve(script));

		// Get a table with the components.
		sol::table entity = lua.get<sol::table>("entity");

		m_name = entity.get<std::string>("name");
		m_isDead = entity.get<bool>("isDead");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		entity.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		m_keyValuePair.erase("name");
		m_keyValuePair.erase("systems");
		m_keyValuePair.erase("isDead");

        RE_REVERSE_ASSERT(m_keyValuePair.empty(), "Attempted to load an entity with no component data", "Entity::init", "Entity.cpp", 40);
        
		// Create list.
		cl[m_name] = ComponentList();

		for (auto& kvp : m_keyValuePair)
		{
			// If we have a physics component, then we automatically create the transformcomponent
			if (kvp.first == "PhysicsComponent")
			{
				auto cf = Locator::get<World>()->getComponentFactory().find("TransformComponent");
				auto end = Locator::get<World>()->getComponentFactory().end();

				if (cf != end)
				{
					cl[m_name][cf->second.first] = cf->second.second();

					// We are passing it the physics component table, because it doesn't matter.
					// Values are automatically fixed in the first update() call and transformcomponent
					// handles any issues that might arise from this.
					cl[m_name][cf->second.first]->init(kvp.second);
				}
			}

			auto cf = Locator::get<World>()->getComponentFactory().find(kvp.first);
			auto end = Locator::get<World>()->getComponentFactory().end();

			if (cf != end)
			{
				cl[m_name][cf->second.first] = cf->second.second();
				cl[m_name][cf->second.first]->init(kvp.second);
			}
		}

		m_components = &cl[m_name];

		sol::table entitySystemList = entity.get<sol::table>("systems");
		std::map<int, std::string> m_systemsKVP;
		entitySystemList.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_systemsKVP.insert({ pair.first.as<int>(), pair.second.as<std::string>() });
		});

		for (auto& skvp : m_systemsKVP)
		{
			for (auto& it : Locator::get<World>()->getSystemList())
			{
				std::string val = skvp.second;
				if (it.second.first == val)
				{
					m_systemIds.emplace(it.second.first, it.first);
				}
			}
		}
	}

	Entity::~Entity()
	{
		m_systemIds.clear();
        m_components = nullptr;
	}

	bool Entity::isDead() const
	{
		return m_isDead;
	}
    
    void Entity::useComponentDebugFunction(const std::string& componentName, sol::table& table, const std::string& curEntityName)
    {
		m_components->at(Locator::get<World>()->m_stringToComponentType.at(componentName))->debugFunction(table, curEntityName);
    }
}
