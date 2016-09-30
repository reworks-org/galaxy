//
//  UILabel.cpp
//  REngine3
//
//  Created by reworks on 12/09/2016.
//  Copyright © 2016 reworks. All rights reserved.
//

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "re/deps/sol2/sol.hpp"

#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "UILabel.hpp"

namespace re
{
	UILabel::UILabel(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		sol::table label = lua.get<sol::table>("UILabel");

		m_xpos = label.get<int>("x");
		m_ypos = label.get<int>("y");
		
		m_hasDropShadow = label.get<int>("dropShadow");

		if (m_hasDropShadow)
		{
			sf::Color temp(0, 0, 0, 255);
			m_text.setOutlineColor(temp);
		}

		m_dropShadowOffset = label.get<float>("dropShadowOffset");

		m_text.setOutlineThickness(m_dropShadowOffset);

		m_fontStream.open(label.get<std::string>("font"));
		m_font.loadFromStream(m_fontStream);
		m_text.setFont(m_font);

		m_text.setString(label.get<std::string>("text"));
		m_text.setCharacterSize(label.get<int>("size"));

		sol::table colour = label.get<sol::table>("colour");

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
		int style = label.get<int>("style");

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

	UILabel::~UILabel()
	{
	}

	void UILabel::SetOffset(int x, int y)
	{
		m_offsetX = x;
		m_offsetY = y;

		m_finalPosX = m_xpos + m_offsetX;
		m_finalPosY = m_ypos + m_offsetY;

		m_text.setPosition((float)m_finalPosX, (float)m_finalPosY);
	}

	void UILabel::Event()
	{
	}

	void UILabel::Update()
	{
	}

	void UILabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_text, states);
	}
}