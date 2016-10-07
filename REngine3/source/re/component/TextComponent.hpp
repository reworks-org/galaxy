//
//  TextComponent.hpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TEXTCOMPONENT_HPP_
#define RENGINE3_TEXTCOMPONENT_HPP_

#include <SFML/Graphics/Text.hpp>

#include "re/types/Component.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class TextComponent : public Component, public sf::Text
	{
	public:
		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		TextComponent(sol::table& table);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the component.
		*/
		~TextComponent() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update the component.
		*/
		void Update(sf::Time dt) override;

	private:
		/*
		* IMPORTS: fontName - Name of the font to use in the VFS.
		* EXPORTS: none
		* PURPOSE: Replaces sf::Text's setFont method. Stores the font data in the class.
		*/
		void LoadFont(const std::string& fontName);

	private:
		sf::Font m_font;
		sf::physfs m_fontStream;

	public:
		unsigned long m_group;
	};
}

#endif