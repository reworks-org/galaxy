//
//  TextComponent.cpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/graphics/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TextComponent.hpp"

namespace re
{
	TextComponent::TextComponent()
	{
        m_group = 0;
	}

	TextComponent::~TextComponent()
	{
	}

	void TextComponent::init(sol::table & table)
	{
		setPosition(table.get<float>("x"), table.get<float>("y"));
		m_group = table.get<sf::Uint32>("group");

		setFont(Locator::get<FontManager>()->get(table.get<std::string>("font")));

		setString(table.get<std::string>("text"));
		setCharacterSize(table.get<int>("size"));

		sol::table colour = table.get<sol::table>("colour");

		sf::Color col;
		col.r = colour.get<sf::Uint8>("r");
		col.g = colour.get<sf::Uint8>("g");
		col.b = colour.get<sf::Uint8>("b");
		col.a = colour.get<sf::Uint8>("a");
		setFillColor(col);

		/*
		0 = Regular,
		1 = Bold,
		2 = Italic,
		3 = Underlined,
		4 = StrikeTrough

		See <SFML/Graphics/Text.hpp>
		*/
		int style = table.get<int>("style");

		switch (style)
		{
		case 0:
			setStyle(sf::Text::Regular);
			break;

		case 1:
			setStyle(sf::Text::Bold);
			break;

		case 2:
			setStyle(sf::Text::Italic);
			break;

		case 3:
			setStyle(sf::Text::Underlined);
			break;

		case 4:
			setStyle(sf::Text::StrikeThrough);
			break;
		}
	}
}
