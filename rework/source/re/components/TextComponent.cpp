//
//  TextComponent.cpp
//  rework
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <allegro5/allegro_font.h>

#include "re/managers/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TextComponent.hpp"

namespace re
{
	TextComponent::TextComponent(sol::table& table)
	{
		m_layer = table.get<unsigned int>("layer");
		m_text = table.get<std::string>("text");
		m_font = Locator::get<FontManager>()->get(table.get<std::string>("font"));

		sol::table colour = table.get<sol::table>("colour");
		m_colour = al_map_rgba(colour.get<unsigned char>("r"), colour.get<unsigned char>("g"), colour.get<unsigned char>("b"), colour.get<unsigned char>("a"));
		
		m_x = table.get<float>("x");
		m_y = table.get<float>("y");
	}

	TextComponent::~TextComponent()
	{
	}

	void TextComponent::draw()
	{
		al_draw_text(m_font, m_colour, m_x, m_y, 0, m_text.c_str());
	}
}