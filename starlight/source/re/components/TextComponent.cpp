///
///  TextComponent.cpp
///  starlight
///
///  Created by reworks on 19/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <allegro5/allegro_font.h>

#include "sol2/sol.hpp"
#include "imgui/imgui_impl_a5.h"
#include "re/managers/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/TransformComponent.hpp"

#include "TextComponent.hpp"

namespace sl
{
	TextComponent::TextComponent(const sol::table& table)
	{
		m_text = table.get<std::string>("text");
		m_font = Locator::get<FontManager>()->get(table.get<std::string>("font"));

		sol::table colour = table.get<sol::table>("colour");
		m_colour = al_map_rgba(colour.get<unsigned char>("r"), colour.get<unsigned char>("g"), colour.get<unsigned char>("b"), colour.get<unsigned char>("a"));
		
		m_offsetX = table.get<float>("offsetX");
		m_offsetY = table.get<float>("offsetY");
		m_layer = table.get<int>("layer");
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
		ImGui::InputFloat("x-pos", &m_offsetX);

		ImGui::Spacing();
		ImGui::InputFloat("y-pos", &m_offsetY);

		ImGui::Spacing();
		if (ImGui::InputInt("Layer: ", &m_layer, 1, 2))
		{
			if (m_layer < 0)
			{
				m_layer = 0;
			}
		}

		ImGui::Spacing();
		ImGui::Text("Colour Modifiers: ");

		ImGui::Spacing();
		ImGui::SliderFloat("Red Editor", &m_colour.r, 0, 255);

		ImGui::Spacing();
		ImGui::SliderFloat("Green Editor", &m_colour.g, 0, 255);

		ImGui::Spacing();
		ImGui::SliderFloat("Blue Editor", &m_colour.b, 0, 255);

		ImGui::Spacing();
		ImGui::SliderFloat("Alpha Editor", &m_colour.a, 0, 255);
	}
}