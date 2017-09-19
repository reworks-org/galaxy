//
//  TransformComponent.cpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/debug/imgui/imgui-sfml.h"
#include "re/physics/Box2DSFMLBridge.hpp"

#include "TransformComponent.hpp"

namespace re
{
	TransformComponent::TransformComponent()
	{
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::init(sol::table& table)
	{
		setPosition(table.get<float>("x"), table.get<float>("y"));
		setRotation(table.get<float>("angle"));
	}

	void TransformComponent::debugFunction(sol::table& table, const std::string& curEntityName)
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
