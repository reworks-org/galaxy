//
//  FontManager.hpp
//  REngine3
//
//  Created by reworks on 15/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_FONTMANAGER_HPP_
#define RENGINE3_FONTMANAGER_HPP_

#include <memory>

#include <SFML/Graphics/Font.hpp>

#include "re/services/ServiceLocator.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class FontManager : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleanup fonts.
		*/
		~FontManager() override;

		/*
		* IMPORTS: name of font and name of file in VFS.
		* EXPORTS: none
		* PURPOSE: Add a font to the map.
		*/
		void Add(const std::string& name, const std::string& fontName);

		/*
		* IMPORTS: name - name of the font in the manager.
		* EXPORTS: sf::Font file.
		* PURPOSE: Retrieve a font from the manager.
		*/
		sf::Font& Get(const std::string& name);

	private:
		std::unordered_map<std::string, std::pair<std::unique_ptr<sf::physfs>, std::shared_ptr<sf::Font>>> m_fontMap;
	};
}

#endif