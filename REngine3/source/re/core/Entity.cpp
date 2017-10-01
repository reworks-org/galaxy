//
//  Entity.cpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/core/World.hpp"
#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

namespace re
{
	Entity::Entity(const std::string& script, ComponentHolder& cl)
	:m_name(""), m_components(nullptr), m_isDead(false)
	{
        m_systemIds.clear();

		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(script));

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

		if (m_keyValuePair.empty())
		{
			RE_LOG(LogLevel::FATAL, "Attempted to load an entity with no component data", "Entity::Entity", "Entity.cpp", 42);
		}

		// Create list.
		cl[m_name] = ComponentList();

		for (auto& kvp : m_keyValuePair)
		{
			// If we have a physics component, then we automatically create the transformcomponent
			if (kvp.first == "PhysicsComponent")
			{
				auto cf = Locator::get<World>()->getComponentFactory();
				auto it = cf.find("TransformComponent");

				if (it != cf.end())
				{
					// We are passing it the physics component table, because it doesn't matter.
					// Values are automatically fixed in the first update() call and transformcomponent
					// handles any issues that might arise from this.
					cl[m_name].emplace(it->second.first, it->second.second(kvp.second));
				}
				else
				{
					RE_LOG(LogLevel::WARNING, "Could not find TransformComponent in the component factory!", "Entity::Entity()", "Entity.cpp", 65);
				}
			}

			auto cf = Locator::get<World>()->getComponentFactory();
			auto it = cf.find(kvp.first);

			if (it != cf.end())
			{
				cl[m_name].emplace(it->second.first, it->second.second(kvp.second));
			}
			else
			{
				std::string out = "Could not find " + kvp.first + "in the component factory!";
				RE_LOG(LogLevel::WARNING, out, "Entity::Entity()", "Entity.cpp", 79);
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
		m_components->at(Locator::get<World>()->m_stringToComponentType.at(componentName))->debug(table, curEntityName);
    }
}
