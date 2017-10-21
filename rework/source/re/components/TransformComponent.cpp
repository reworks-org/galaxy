//
//  TransformComponent.cpp
//  rework
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "imgui/imgui.h"

#include "TransformComponent.hpp"

namespace re
{
	TransformComponent::TransformComponent(sol::table& table)
	{
		m_x = table.get<float>("x");
		m_y = table.get<float>("y");
		m_angle = table.get<float>("angle");
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::debug()
	{
		ImGui::InputFloat("X Pos", &m_x);

		ImGui::Spacing();
		ImGui::InputFloat("Y Pos", &m_y);

		ImGui::Spacing();
		ImGui::SliderAngle("Angle Modifier", &m_angle, 0.0f, 360.0f);
	}
}