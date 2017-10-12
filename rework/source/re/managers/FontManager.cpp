//
//  FontManager.cpp
//  rework
//
//  Created by reworks on 22/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <map>

#include <allegro5/allegro_ttf.h>

#include "sol2/sol.hpp"
#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "FontManager.hpp"

namespace re
{
	FontManager::FontManager(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(script));
		sol::table fonts = lua.get<sol::table>("fonts");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		fonts.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& it : m_keyValuePair)
		{
			std::string fn = it.second.get<std::string>("font");
			m_fontMap.emplace(it.first, al_load_ttf_font_f(Locator::get<VFS>()->open(fn, "r"), fn.c_str(), it.second.get<int>("size"), NULL));
		}
	}

	FontManager::~FontManager()
	{
		for (auto& it : m_fontMap)
		{
			al_destroy_font(it.second);
		}

		m_fontMap.clear();
	}

	ALLEGRO_FONT* FontManager::get(const std::string& id)
	{
		return m_fontMap[id];
	}
}