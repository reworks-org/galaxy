//
//  TextureAtlas.cpp
//  REngine3
//
//  Created by reworks on 13/01/2017.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "TextureAtlas.hpp"

namespace re
{
	TextureAtlas::~TextureAtlas()
	{
		m_atlas.clear();
	}

	void TextureAtlas::AddTexture(const std::string& texture, const std::string& id)
	{
		m_atlas.emplace(id, std::make_pair(sf::physfs(), sf::Texture()));
		m_atlas[id].first.open(texture);
		m_atlas[id].second.loadFromStream(m_atlas[id].first);
	}

	sf::Texture& TextureAtlas::GetTexture(const std::string& id)
	{
		return m_atlas[id].second;
	}
}