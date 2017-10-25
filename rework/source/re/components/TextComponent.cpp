//
//  TextComponent.cpp
//  rework
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>

#include "entityx/Entity.h"
#include "imgui/imgui_impl_a5.h"
#include "re/managers/FontManager.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/TransformComponent.hpp"

#include "TextComponent.hpp"

namespace re
{
	TextComponent::TextComponent(ex::Entity& e, sol::table& table)
	{
		m_text = table.get<std::string>("text");
		m_font = Locator::get<FontManager>()->get(table.get<std::string>("font"));

		sol::table colour = table.get<sol::table>("colour");
		m_colour = al_map_rgba(colour.get<unsigned char>("r"), colour.get<unsigned char>("g"), colour.get<unsigned char>("b"), colour.get<unsigned char>("a"));
		
		m_offsetX = table.get<float>("offsetX");
		m_offsetY = table.get<float>("offsetY");
		m_layer = table.get<int>("layer");
		m_entity = e;
	}

	TextComponent::~TextComponent()
	{
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
				m_layer = 0;
		}

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

	void TextComponent::render()
	{
		al_draw_text(m_font, m_colour, m_entity.component<TransformComponent>()->m_x + m_offsetX, m_entity.component<TransformComponent>()->m_y+ m_offsetY, 0, m_text.c_str());
	}
}