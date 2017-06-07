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
		sol::object x = table["x"];
		sol::object y = table["y"];
		sol::object a = table["angle"];

		if (x != sol::nil && y != sol::nil && a != sol::nil)
		{
			setPosition(table.get<float>("x"), table.get<float>("y"));
			setRotation(table.get<float>("angle"));
		}
	}

	void TransformComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static float x = getPosition().x;
		static float y = getPosition().y;
		static float angle = b2::degToRad<float>(getRotation());
		static std::string originalEntityName = curEntityName;

		if (originalEntityName != curEntityName)
		{
			originalEntityName = curEntityName;
			x = getPosition().x;
			y = getPosition().y;
			angle = b2::degToRad<float>(getRotation());
		}

		ImGui::Spacing();
		ImGui::InputFloat("x pixel pos", &x);

		ImGui::Spacing();
		ImGui::InputFloat("y pixel pos", &y);

		setPosition(x, y);

		ImGui::Spacing();
		ImGui::SliderAngle("Angle Modifier", &angle, 0.0f, 360.0f);

		setRotation(b2::radToDeg<float>(angle));
	}
}
