//
//  SpriteComponent.cpp
//  rework
//
//  Created by reworks on 10/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>

#include "imgui/imgui_impl_a5.h"

#include "SpriteComponent.hpp"

namespace re
{
	SpriteComponent::SpriteComponent(sol::table& table)
	{
		m_spriteName = table.get<std::string>("spriteName");	
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::debug()
	{
		ImGui::stl::InputText("Sprite Name (in atlas):", &m_spriteName, ImGuiInputTextFlags_EnterReturnsTrue);
	}
}