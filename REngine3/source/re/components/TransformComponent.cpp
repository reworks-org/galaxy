//
//  TransformComponent.cpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/libs/imgui/imgui.h"

#include "TransformComponent.hpp"

namespace re
{
	TransformComponent::TransformComponent(sol::table& table)
	{
		m_x = table.get<float>("x");
		m_y = table.get<float>("y");
		m_rect.width = table.get<float>("width");
		m_rect.height = table.get<float>("height");
		m_angle = table.get<float>("angle");
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::debug(sol::table& table, const std::string& curEntityName)
	{
		static float x = m_rect.x;
		static float y = m_rect.y;
		static int w = m_rect.width;
		static int h = m_rect.height;
		static float angle = b2::degToRad<float>(m_angle);
		static std::string originalEntityName = curEntityName;
		static bool updatePos = true;

		if (originalEntityName != curEntityName)
		{
			originalEntityName = curEntityName;
			x = m_rect.x;
			y = m_rect.y;
			angle = b2::degToRad<float>(m_angle);
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::InputFloat("X Modifier", &x))
		{
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::InputFloat("Y Modifier", &y))
		{
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::InputInt("Width Modifier", &w, 1, 2))
		{
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::InputInt("Height Modifier", &h, 1, 2))
		{
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::SliderAngle("Angle Modifier", &angle, 0.0f, 360.0f))
		{
			updatePos = true;
		}

		if (updatePos)
		{
			m_rect.x = x;
			m_rect.y = y;
			m_rect.width = w;
			m_rect.height = h;
			m_angle = b2::radToDeg<float>(angle);

			updatePos = false;
		}
	}
}
