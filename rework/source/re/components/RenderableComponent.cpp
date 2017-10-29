//
//  RenderableComponent.hpp
//  rework
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "imgui/imgui_impl_a5.h"

#include "RenderableComponent.hpp"

namespace re
{
	RenderableComponent::RenderableComponent(entityx::Entity& e, sol::table& table)
	{
	}

	RenderableComponent::~RenderableComponent()
	{
	}

	void RenderableComponent::debug()
	{
		ImGui::Text("Flag class. Nothing else.");
	}
}