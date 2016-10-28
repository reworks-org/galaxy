//
//  Level.cpp
//  REngine3
//
//  Created by reworks on 24/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/services/vfs/VFS.hpp"
#include "re/scripting/sol2/sol.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Level.hpp"

namespace re
{
	Level::Level(const std::string& script, sf::View camera)
		:m_camera(camera)
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));
		sol::table level = lua.get<sol::table>("level");

		m_maploader = std::make_shared<tmx::MapLoader>(level.get<std::string>("dir"));
		m_maploader->loadFromMemory(Locator::Get<VFS>()->ToString(level.get<std::string>("startingMap")));
	}

	void Level::Move(float x, float y)
	{
		m_camera.move(x, y);
	}

	void Level::ChangeCurrentMap(const std::string& map)
	{
		m_maploader->loadFromMemory(Locator::Get<VFS>()->ToString(map));
	}
}