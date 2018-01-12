///
///  TransformComponent.cpp
///  rework
///
///  Created by reworks on 30/09/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "sol2/sol.hpp"
#include "imgui/imgui_impl_a5.h"

#include "TransformComponent.hpp"

namespace re
{
	TransformComponent::TransformComponent(const sol::table& table)
	{
		m_rect.m_x = table.get<float>("x");
		m_rect.m_y = table.get<float>("y");
		m_rect.m_width = table.get<int>("width");
		m_rect.m_height = table.get<int>("height");
		m_angle = table.get<float>("angle");
	}

	void TransformComponent::debug()
	{
		ImGui::InputFloat("X Pos", &m_rect.m_x);

		ImGui::Spacing();
		ImGui::InputFloat("Y Pos", &m_rect.m_y);

		ImGui::Spacing();
		ImGui::InputInt("Width Modifier", &m_rect.m_width, 1, 2);

		ImGui::Spacing();
		ImGui::InputInt("Height Modifier", &m_rect.m_height, 1, 2);

		ImGui::Spacing();
		ImGui::SliderAngle("Angle Modifier", &m_angle, 0.0f, 360.0f);
	}
}