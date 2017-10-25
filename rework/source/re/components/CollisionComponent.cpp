//
//  CollisionComponent.cpp
//  rework
//
//  Created by reworks on 10/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "imgui/imgui.h"
#include "entityx/Entity.h"

#include "CollisionComponent.hpp"

namespace re
{
	CollisionComponent::CollisionComponent(ex::Entity& e, sol::table& table)
	{
		m_width = table.get<int>("width");
		m_height = table.get<int>("height");
	}

	CollisionComponent::~CollisionComponent()
	{
	}

	void CollisionComponent::debug()
	{
		ImGui::InputInt("Width Modifier", &m_width, 1, 2);

		ImGui::Spacing();
		ImGui::InputInt("Height Modifier", &m_height, 1, 2);
	}
}