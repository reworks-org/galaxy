//
//  CollisionComponent.cpp
//  rework
//
//  Created by reworks on 10/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "imgui/imgui_impl_a5.h"

#include "CollisionComponent.hpp"

namespace re
{
	CollisionComponent::CollisionComponent(entityx::Entity& e, sol::table& table)
	{
	}

	CollisionComponent::~CollisionComponent()
	{
	}

	void CollisionComponent::debug()
	{
		ImGui::Text("Flag class. Nothing editable.");
	}
}