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
	:m_group(0)
	{
	}

	TextComponent::~TextComponent()
	{
	}

	void TextComponent::init(sol::table& table)
	{
		m_text.setPosition(table.get<float>("x"), table.get<float>("y"));
		m_group = table.get<sf::Uint32>("group");

		m_text.setFont(Locator::get<ResourceManager<sf::Font>>()->get(table.get<std::string>("font")));

		m_text.setString(table.get<std::string>("text"));
		m_text.setCharacterSize(table.get<int>("size"));

		sol::table colour = table.get<sol::table>("colour");

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

	void TextComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static float xpos = m_text.getPosition().x;
		static float ypos = m_text.getPosition().y;
		static int r = m_text.getFillColor().r;
		static int g = m_text.getFillColor().g;
		static int b = m_text.getFillColor().b;
		static int alpha = m_text.getFillColor().a;
		static int fs = m_text.getCharacterSize();
		static int style = table.get<int>("style");
		static std::string original = table.get<std::string>("text");
		static std::string originalFont = table.get<std::string>("font");
		static std::vector<char> fontinput{ originalFont.begin(), originalFont.end() };
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
			xpos = m_text.getPosition().x;
			ypos = m_text.getPosition().y;
			r = m_text.getFillColor().r;
			g = m_text.getFillColor().g;
			b = m_text.getFillColor().b;
			alpha = m_text.getFillColor().a;
			fs = m_text.getCharacterSize();
			style = table.get<int>("style");

			fontinput.clear();
			fontinput = std::vector<char>(originalFont.begin(), originalFont.end());
			fontinput.resize(255);
		}

		ImGui::Text("%s", std::string("Group: " + std::to_string(m_group)).c_str());

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

			m_text.setFont(Locator::get<ResourceManager<sf::Font>>()->get(newFont));
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

		if (ImGui::SliderInt("Red Editor", &r, 0, 255))
		{
			updatePos = true;
		}

		if (ImGui::SliderInt("Green Editor", &g, 0, 255))
		{
			updatePos = true;
		}

		if (ImGui::SliderInt("Blue Editor", &b, 0, 255))
		{
			updatePos = true;
		}

		if (ImGui::SliderInt("Alpha Editor", &alpha, 0, 255))
		{
			updatePos = true;
		}

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

			m_text.setPosition(xpos, ypos);
			m_text.setCharacterSize(fs);
			m_text.setFillColor(sf::Color(r, g, b, alpha));
		}

        ImGui::Spacing();
        ImGui::SliderInt("Text Style", &style, 0, 4);
        
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
}
