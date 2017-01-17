//
//  TextureAtlas.hpp
//  REngine3
//
//  Created by reworks on 13/01/2017.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TEXTUREATLAS_HPP_
#define RENGINE3_TEXTUREATLAS_HPP_

#include <vector>

#include <SFML/Graphics/Texture.hpp>

#include "re/types/Service.hpp"

namespace re
{
	class TextureAtlas : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		TextureAtlas(unsigned long maxTextures);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the atlas.
		*/
		~TextureAtlas() override;

		/*
		* IMPORTS: texture file name in VFS, index to add it to.
		* EXPORTS: none
		* PURPOSE: Add a shared texture to the atlas.
		*/
		void AddTexture(const std::string& texture, unsigned long index);

		/*
		* IMPORTS: atlasIndex
		* EXPORTS: none
		* PURPOSE: Get a texture from the atlas.
		*/
		sf::Texture& GetTexture(unsigned long atlasIndex);

	private:
		std::vector<sf::Texture> m_atlas;
	};
}

#endif