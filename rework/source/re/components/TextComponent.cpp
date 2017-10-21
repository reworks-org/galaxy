//
//  TextComponent.cpp
//  rework
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>

#include "imgui/imgui_impl_a5.h"
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
		
		ImGui::stl::InputText("Text: ", &m_text, ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::Spacing();

		std::string font = "";
		if (ImGui::stl::InputText("Font Picker", &font, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_font = Locator::get<FontManager>()->get(font);
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

		int r, g, b, a = 0;
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