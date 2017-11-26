//
//  Level.cpp
//  rework
//
//  Created by reworks on 21/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "sol2/sol.hpp"
#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Level.hpp"

namespace re
{
	Level::Level(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(script));
		sol::table level = lua.get<sol::table>("level");

		std::string mapData = Locator::get<VFS>()->openAsString("map");
		m_map = tmx_load_buffer(mapData.c_str(), mapData.length());

		m_dimensions.x = level.get<float>("x");
		m_dimensions.y = level.get<float>("y");
		m_dimensions.width = level.get<int>("width");
		m_dimensions.height = level.get<int>("height");
	}

	Level::~Level()
	{
		tmx_map_free(m_map);
	}

	Rect<int>& Level::getDimensions() const
	{
		return m_dimensions;
	}
}