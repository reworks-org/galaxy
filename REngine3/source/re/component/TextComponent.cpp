//
//  TextComponent.cpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TextComponent.hpp"

namespace re
{
	TextComponent::TextComponent(sol::table& table)
	{
		m_text.setPosition({ table.get<float>("x"), table.get<float>("y") });
		m_group = table.get<unsigned long>("group");

		fontData.open(table.get<std::string>("font"));
		m_font.loadFromStream(fontData);

		m_text.setFont(m_font);

		m_text.setString(table.get<std::string>("text"));
		m_text.setCharacterSize(table.get<int>("size"));

		sol::table colour = table["colour"];
		
		sf::Color col;
		col.r = colour.get<sf::Uint8>("r");
		col.g = colour.get<sf::Uint8>("g");
		col.b = colour.get<sf::Uint8>("b");
		col.a = colour.get<sf::Uint8>("a");
		m_text.setFillColor(col);

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
			m_text.setStyle(sf::Text::Regular);
			break;

		case 1:
			m_text.setStyle(sf::Text::Bold);
			break;

		case 2:
			m_text.setStyle(sf::Text::Italic);
			break;

		case 3:
			m_text.setStyle(sf::Text::Underlined);
			break;

		case 4:
			m_text.setStyle(sf::Text::StrikeThrough);
			break;
		}
	}

	TextComponent::~TextComponent()
	{
	}
}