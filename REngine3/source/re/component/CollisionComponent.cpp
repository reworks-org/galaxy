//
//  CollisionComponent.cpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/debug/imgui/imgui-sfml.h"

#include "CollisionComponent.hpp"

namespace re
{
	CollisionComponent::CollisionComponent()
	{
	}

	CollisionComponent::~CollisionComponent()
	{
	}

	void CollisionComponent::init(sol::table& table)
	{
		m_width = table.get<int>("width");
		m_height = table.get<int>("height");
	}

	void CollisionComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		ImGui::Spacing();
		ImGui::InputInt("Width Modifier", &m_width, 1, 2);

		ImGui::Spacing();
		ImGui::InputInt("Height Modifier", &m_height, 1, 2);
	}
}
