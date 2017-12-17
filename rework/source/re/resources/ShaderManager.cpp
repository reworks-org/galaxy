//
//  ShaderManager.cpp
//  rework
//
//  Created by reworks on 14/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <map>

#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "ShaderManager.hpp"

namespace re
{
	ShaderManager::ShaderManager(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(script));
		sol::table shaders = lua.get<sol::table>("shaders");

		std::map<std::string, sol::table> kvp;
		shaders.for_each([&](std::pair<sol::object, sol::object> pair) {
			kvp.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& it : kvp)
		{
			m_shaderMap.emplace(it.first, it.second);
		}
	}

	ShaderManager::~ShaderManager()
	{
		m_shaderMap.clear();
	}

	Shader* ShaderManager::get(const std::string& name)
	{
		return &(m_shaderMap.at(name));
	}
}