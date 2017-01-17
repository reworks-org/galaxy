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
	TextureAtlas::TextureAtlas(unsigned long maxTextures)
	{
		m_atlas.resize(0);
		m_atlas.clear();

		m_atlas.reserve(maxTextures);
	}

	TextureAtlas::~TextureAtlas()
	{
		m_atlas.clear();
	}

	void TextureAtlas::AddTexture(const std::string & texture, unsigned long index)
	{
		
	}

	sf::Texture& TextureAtlas::GetTexture(unsigned long atlasIndex)
	{
		return m_atlas[atlasIndex];
	}
}