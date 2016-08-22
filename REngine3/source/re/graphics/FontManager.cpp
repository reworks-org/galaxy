//
//  FontManager.cpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "FontManager.hpp"
 
namespace re
{
	FontManager::~FontManager()
	{
		m_fonts.clear();
	}

	void FontManager::Add(sf::Font& font, const std::string& id, const std::string& fontFile)
	{
		font.loadFromMemory(Locator::Get<VFS>()->ToString(fontFile).c_str(), Locator::Get<VFS>()->ToString(fontFile).size());
		m_fonts.emplace(id, font);
	}

	sf::Font& FontManager::Get(const std::string& id)
	{
		return m_fonts[id];
	}
}