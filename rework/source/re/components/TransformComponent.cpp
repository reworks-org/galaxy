//
//  TransformComponent.cpp
//  rework
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "imgui/imgui_impl_a5.h"

#include "TransformComponent.hpp"

namespace re
{
	TransformComponent::TransformComponent(entityx::Entity& e, sol::table& table)
	{
		m_rect.x = table.get<float>("x");
		m_rect.y = table.get<float>("y");
		m_rect.width = table.get<int>("width");
		m_rect.height = table.get<int>("height");
		m_angle = table.get<float>("angle");
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::debug()
	{
		ImGui::InputFloat("X Pos", &m_rect.x);

		ImGui::Spacing();
		ImGui::InputFloat("Y Pos", &m_rect.y);

		ImGui::Spacing();
		ImGui::InputInt("Width Modifier", &m_rect.width, 1, 2);

		ImGui::Spacing();
		ImGui::InputInt("Height Modifier", &m_rect.height, 1, 2);

		ImGui::Spacing();
		ImGui::SliderAngle("Angle Modifier", &m_angle, 0.0f, 360.0f);
	}
}