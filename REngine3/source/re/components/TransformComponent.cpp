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
		m_angle = table.get<float>("angle");
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::debug(sol::table& table, const std::string& curEntityName)
	{
		static float x = getPosition().x;
		static float y = getPosition().y;
		static float angle = b2::degToRad<float>(getRotation());
		static std::string originalEntityName = curEntityName;
		static bool updatePos = true;

		if (originalEntityName != curEntityName)
		{
			originalEntityName = curEntityName;
			x = getPosition().x;
			y = getPosition().y;
			angle = b2::degToRad<float>(getRotation());
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::InputFloat("x pixel pos", &x))
		{
			updatePos = true;
		}

		ImGui::Spacing();
		if (ImGui::InputFloat("y pixel pos", &y))
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
			setPosition(x, y);
			setRotation(b2::radToDeg<float>(angle));

			updatePos = false;
		}
	}
}
