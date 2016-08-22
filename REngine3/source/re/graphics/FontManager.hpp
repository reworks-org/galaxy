//
//  FontManager.hpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_FONTMANAGER_HPP_
#define RENGINE3_FONTMANAGER_HPP_

#include <unordered_map>

#include <SFML/Graphics/Font.hpp>

#include "re/services/Service.hpp"

namespace re
{
	class FontManager : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the fonts.
		*/
		~FontManager() override;

		/*
		* IMPORTS: A new sf::Font and the assosiated id.
		* EXPORTS: none
		* PURPOSE: Register a font into the game system.
		*/
		void Add(sf::Font& font, const std::string& id, const std::string& fontFile);

		/*
		* IMPORTS: The ID of the font you want to retrieve.
		* EXPORTS: none
		* PURPOSE: Get a font from the system.
		*/
		sf::Font& Get(const std::string& id);

	private:
		std::unordered_map<std::string, sf::Font> m_fonts;
	};
}

#endif