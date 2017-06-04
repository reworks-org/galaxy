//
//  TextComponent.cpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/debug/imgui/imgui.h"
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

	void TextComponent::init(sol::table& table)
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

	void TextComponent::debugFunction(sol::table& table)
	{
		static int xpos = getPosition().x;
		static int ypos = getPosition().y;
        static int r = getFillColor().r;
        static int g = getFillColor().g;
        static int b = getFillColor().b;
		static int alpha = getFillColor().a;
        static int fs = getCharacterSize();
        static int style = table.get<int>("style");
        static std::string original = table.get<std::string>("text");
        static std::vector<std::string> fonts;
        
        for (auto& v : Locator::get<Font>())
        {
            
        }
        
        
        if (original != table.get<std::string>("text"))
        {
            original = table.get<std::string>("text");
            // update variables
        }
        
		ImGui::Spacing();
		ImGui::Text(std::string("Group: " + std::to_string(m_group)).c_str());

        ImGui::Spacing();
        ImGui::Text("Position Modifiers: ");
        
		ImGui::Spacing();
		ImGui::InputInt("x-pos", &xpos);

		ImGui::Spacing();
		ImGui::InputInt("y-pos", &ypos);

		setPosition(xpos, ypos);
        
        ImGui::Spacing();
        ImGui::Text("Colour Modifiers: ");

        ImGui::SliderInt("Red Editor", &r, 0, 255);
        ImGui::SliderInt("Green Editor", &g, 0, 255);
        ImGui::SliderInt("Blue Editor", &b, 0, 255);
		ImGui::SliderInt("Alpha Editor", &alpha, 0, 255);

        setFillColor(sf::Color(r, g, b, alpha));
        
        ImGui::Spacing();
        ImGui::Text("Other Modifiers: ");
        
        ImGui::Spacing();
        ImGui::InputInt("Font Size", &fs);
        setCharacterSize(fs);
        
        ImGui::Spacing();
        ImGui::SliderInt("Text Style", &style, 0, 4);
        
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
