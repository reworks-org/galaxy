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
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "FontBook.hpp"

namespace sl
{
	FontBook::FontBook(const std::string& script)
	{
		// Load lua script from VFS.
		sol::state loader;
		loader.script(Locator::virtualFS->openAsString(script));
		sol::table fonts = loader.get<sol::table>("fonts");

		if (!fonts.empty())
		{
			// Iterate over each font in the lua table and construct it in place.
			fonts.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				auto valueTable = pair.second.as<sol::table>();
				ALLEGRO_FONT* font = al_load_ttf_font(valueTable.get<const char*>("font"), valueTable.get<int>("size"), NULL);

				if (!font)
				{
					LOG_S(ERROR) << "Could not create font : " << valueTable.get<const char*>("font") << ". Allegro Errno: " << al_get_errno();
				}
				else
				{
					m_resourceMap.emplace(pair.first.as<const char*>(), font);
				}
			});
		}
		else
		{
			LOG_S(WARNING) << "Loaded empty font table for script: " << script;
		}
	}

	FontBook::~FontBook()
	{
		// See clean().
		clean();
	}
	
	void FontBook::clean()
	{
		// Iterate over each font in the map and ensure its destroyed properly.
		for (auto& it : m_resourceMap)
		{
			al_destroy_font(it.second);
		}

		m_resourceMap.clear();
	}
}