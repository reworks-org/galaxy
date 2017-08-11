//
//  PhysicsComponent.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>
#include "re/scripting/lua/lua.hpp"

#include "re/debug/imgui/imgui-SFML.h"
#include "re/services/ServiceLocator.hpp"

#include "PhysicsComponent.hpp"

namespace re
{
	PhysicsComponent::PhysicsComponent()
	{
	}

	PhysicsComponent::~PhysicsComponent()
	{
		
	}

	void PhysicsComponent::init(sol::table& table)
	{

	}

	void PhysicsComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		ImGui::Text("Edit the script to make changes.");
		
		ImGui::Spacing();

		ImGui::Text("This is done because there are issues with manually setting stuff in box2d.");
	}
}
