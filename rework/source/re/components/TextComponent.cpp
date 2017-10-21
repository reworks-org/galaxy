//
//  TextComponent.cpp
//  rework
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>

#include "imgui/imgui.h"
#include "re/managers/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TextComponent.hpp"

namespace re
{
	TextComponent::TextComponent(sol::table& table)
	{
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

	void TextComponent::debug()
	{
		static bool update = false;

		char buff[1024];
		if (ImGui::InputText("Text:", &buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_text = buff;
			m_text.erase(std::remove_if(m_text.begin(), m_text.end(), '\r'), m_text.end());
			m_text.erase(std::remove_if(m_text.begin(), m_text.end(), '\n'), m_text.end());

			m_text.shrink_to_fit();
		}

		ImGui::Spacing();

		char fontbuff[1024];
		if (ImGui::InputText("Font Picker", &fontbuff, sizeof(fontbuff), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::string font_temp = fontbuff;
			font_temp.erase(std::remove_if(font_temp.begin(), font_temp.end(), isspace), font_temp.end());
			font_temp.erase(std::remove_if(font_temp.begin(), font_temp.end(), '\r'), font_temp.end());
			font_temp.erase(std::remove_if(font_temp.begin(), font_temp.end(), '\n'), font_temp.end());

			font_temp.shrink_to_fit();

			m_font = Locator::get<FontManager>()->get(font_temp);
		}

		ImGui::Spacing();
		ImGui::Text("Position Modifiers: ");
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::InputFloat("x-pos", &m_x);

		ImGui::Spacing();
		ImGui::InputFloat("y-pos", &m_y);

		ImGui::Spacing();
		ImGui::Text("Colour Modifiers: ");

		unsigned int r, g, b, a = 0;
		if (ImGui::SliderInt("Red Editor", &r, 0, 255))
		{
			update = true;
		}

		if (ImGui::SliderInt("Green Editor", &g, 0, 255))
		{
			update = true;
		}

		if (ImGui::SliderInt("Blue Editor", &b, 0, 255))
		{
			update = true;
		}

		if (ImGui::SliderInt("Alpha Editor", &a, 0, 255))
		{
			update = true;
		}

		if (update)
		{
			m_colour = al_map_rgba(r, g, b, a);
			update = false;
		}
	}
}