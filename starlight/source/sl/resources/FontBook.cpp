///
///  FontBook.cpp
///  starlight
///
///  Created by reworks on 22/09/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_ttf.h>

#include "sol2/sol.hpp"
#include "sl/fs/VFS.hpp"

#include "FontBook.hpp"

namespace sl
{
	FontBook::FontBook(const std::string& script)
	{
		sol::state lua;
		lua.script(VFS::inst()->openAsString(script));
		sol::table fonts = lua.get<sol::table>("fonts");

		fonts.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			auto key = pair.first.as<entt::HashedString>();
			auto value = pair.second.as<sol::table>();

			m_resourceMap.emplace(key, al_load_ttf_font(value.get<std::string>("font").c_str(), value.get<int>("size"), NULL));
		});
	}

	FontBook::~FontBook()
	{
		clean();
	}
	
	void FontBook::clean()
	{
		for (auto& it : m_resourceMap)
		{
			al_destroy_font(it.second);
		}

		m_resourceMap.clear();
	}
}