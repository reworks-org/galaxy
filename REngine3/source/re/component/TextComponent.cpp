//
//  TextComponent.cpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/graphics/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TextComponent.hpp"

namespace re
{
	TextComponent::TextComponent(sol::table& table)
	{
		m_text.setFont(Locator::Get<FontManager>()->Get(table["font"]));
		m_text.setString(table["text"]);
		m_text.setCharacterSize(table["size"]);

		sol::table colour = table["colour"];
		
		sf::Color col;
		col.r = colour["r"];
		col.g = colour["g"];
		col.b = colour["b"];
		col.a = colour["a"];
		m_text.setFillColor(col);

		/*
		0 = Regular,
		1 = Bold,
		2 = Italic,
		3 = Underlined,
		4 = StrikeTrough

		See <SFML/Graphics/Text.hpp>
		*/
		int style = table["style"];

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