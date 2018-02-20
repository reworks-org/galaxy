///
/// FontBook.cpp
/// starlight
///
/// Created by reworks on 22/09/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_ttf.h>

#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "FontBook.hpp"

namespace sl
{
	FontBook::FontBook(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::m_virtualFS->openAsString(script));
		sol::table fonts = lua.get<sol::table>("fonts");

		if (!fonts.empty())
		{
			fonts.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				auto key = entt::HashedString{ pair.first.as<const char*>() };
				auto value = pair.second.as<sol::table>();

				m_resourceMap.emplace(key, al_load_ttf_font(value.get<const char*>("font"), value.get<int>("size"), NULL));
			});
		}
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