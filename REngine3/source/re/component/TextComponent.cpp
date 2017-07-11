//
//  TextComponent.cpp
//  REngine3
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"
#include "re/services/VFS.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/debug/imgui/imgui-sfml.h"
#include "re/utility/ResourceManager.hpp"
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

		setFont(Locator::get<ResourceManager<sf::Font>>()->get(table.get<std::string>("font")));

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

	void TextComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static float xpos = getPosition().x;
		static float ypos = getPosition().y;
        static int r = getFillColor().r;
        static int g = getFillColor().g;
        static int b = getFillColor().b;
		static int alpha = getFillColor().a;
        static int fs = getCharacterSize();
        static int style = table.get<int>("style");
        static std::string original = table.get<std::string>("text");  
		static std::string originalFont = table.get<std::string>("font");
		static std::vector<char> fontinput { originalFont.begin(), originalFont.end() };
		static bool doneOnce = false;
		static std::string originalEntityName = curEntityName;
		static bool updatePos = true;

		if (!doneOnce)
		{
			fontinput.resize(255);
			doneOnce = true;
		}

        if (originalEntityName != curEntityName)
        {
			originalEntityName = curEntityName;
			updatePos = true;
            original = table.get<std::string>("text");
			originalFont = table.get<std::string>("font");
			xpos = getPosition().x;
			ypos = getPosition().y;
			r = getFillColor().r;
			g = getFillColor().g;
			b = getFillColor().b;
			alpha = getFillColor().a;
			fs = getCharacterSize();
			style = table.get<int>("style");

			fontinput.clear();
			fontinput = std::vector<char>(originalFont.begin(), originalFont.end());
			fontinput.resize(255);
        }
      
		ImGui::Text(std::string("Group: " + std::to_string(m_group)).c_str());

		ImGui::Spacing();
		if (ImGui::InputText("Font Picker", fontinput.data(), fontinput.size(), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			for (auto& v : fontinput)
			{
				if (!v)
				{
					v = ' ';
				}
			}

			std::string newFont(fontinput.begin(), fontinput.end());
			fontinput.clear();

			newFont.erase(std::remove_if(newFont.begin(), newFont.end(), isspace), newFont.end());
			newFont.shrink_to_fit();

			fontinput = std::vector<char>(newFont.begin(), newFont.end());
			fontinput.resize(255);

			setFont(Locator::get<ResourceManager<sf::Font>>()->get(newFont));
		}

        ImGui::Spacing();
        ImGui::Text("Position Modifiers: ");
        
		ImGui::Spacing();
		if (ImGui::InputFloat("x-pos", &xpos))
		{
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::InputFloat("y-pos", &ypos))
		{
			updatePos = true;
		}
        
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
		if (ImGui::InputInt("Font Size", &fs))
		{
			updatePos = true;
		}
        
        
		if (updatePos)
		{
			updatePos = false;

			setPosition(xpos, ypos);
			setCharacterSize(fs);
		}

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
