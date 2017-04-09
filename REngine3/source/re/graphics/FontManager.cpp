//
//  FontManager.cpp
//  REngine3
//
//  Created by reworks on 15/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "FontManager.hpp"

namespace re
{
	FontManager::~FontManager()
	{
		m_fontMap.clear();
	}

	void FontManager::add(const std::string& name, const std::string& fontName)
	{
		m_fontMap.emplace(name, std::make_pair(std::make_unique<sf::physfs>(), std::make_shared<sf::Font>()));
		m_fontMap[name].first->open(fontName);
		m_fontMap[name].second->loadFromStream(*(m_fontMap[name].first));
	}

	sf::Font& FontManager::get(const std::string& name)
	{
		return *(m_fontMap[name].second);
	}
}
