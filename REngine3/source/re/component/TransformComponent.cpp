//
//  TransformComponent.cpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/debug/imgui/imgui-sfml.h"

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
		std::string x = "x pixel pos: " + std::to_string(getPosition().x);
		std::string y = "y pixel pos: " + std::to_string(getPosition().y);
		std::string angle = "angle: " + std::to_string(getRotation());

		ImGui::Text(x.c_str());
		ImGui::Spacing();

		ImGui::Text(y.c_str());
		ImGui::Spacing();

		ImGui::Text(angle.c_str());
	}
}
