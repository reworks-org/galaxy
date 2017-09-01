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
		m_rect.width = table.get<int>("width");
		m_rect.height = table.get<int>("height");
	}

	void CollisionComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static int w = m_rect.width;
		static int h = m_rect.height;
		static std::string originalEntityName = curEntityName;
		static bool updatePos = true;

		if (originalEntityName != curEntityName)
		{
			originalEntityName = curEntityName;
			w = m_rect.width;
			h = m_rect.height;
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

		if (updatePos)
		{
			m_rect.width = w;
			m_rect.height = h;

			updatePos = false;
		}
	}
}
