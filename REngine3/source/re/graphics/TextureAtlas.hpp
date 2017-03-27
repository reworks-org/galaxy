//
//  TextureAtlas.hpp
//  REngine3
//
//  Created by reworks on 13/01/2017.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TEXTUREATLAS_HPP_
#define RENGINE3_TEXTUREATLAS_HPP_

#include <map>

#include <SFML/Graphics/Texture.hpp>

#include "re/types/Service.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class TextureAtlas : public Service
	{
	public:
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
		void add(const std::string& texture, const std::string& id);

		/*
		* IMPORTS: atlasIndex
		* EXPORTS: none
		* PURPOSE: Get a texture from the atlas.
		*/
		sf::Texture& get(const std::string& id);

	private:
		std::map<std::string, std::pair<sf::physfs, sf::Texture>> m_atlas;
	};
}

#endif
