//
//  Level.cpp
//  REngine3
//
//  Created by reworks on 24/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Level.hpp"

namespace re
{
	Level::Level(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));
		sol::table level = lua.get<sol::table>("level");
		sol::table maps = level.get<sol::table>("maps");

		// Get key-value pairs from table
		std::map<std::string, std::string> m_keyValuePair;
		maps.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		for (auto& kvp : m_keyValuePair)
		{
			m_tileMaps.emplace(kvp.first, std::make_shared<tmx::TileMap>(kvp.second));
		}

		m_currentMap = level.get<std::string>("currentMap");
	}

	void Level::Move(float x, float y)
	{
		m_camera.move(x, y);
	}

	void Level::ChangeCurrentMap(const std::string& map)
	{
		m_currentMap = map;
	}
}