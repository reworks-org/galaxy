//
//  SpriteComponent.cpp
//  rework
//
//  Created by reworks on 10/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>

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
		char[1024] buff;
		if (ImGui::InputText("Sprite Name (in atlas):", &buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_spriteName = buff;

			m_spriteName.erase(std::remove_if(m_spriteName.begin(), m_spriteName.end(), isspace), m_spriteName.end());
			m_spriteName.erase(std::remove_if(m_spriteName.begin(), m_spriteName.end(), '\r'), m_spriteName.end());
			m_spriteName.erase(std::remove_if(m_spriteName.begin(), m_spriteName.end(), '\n'), m_spriteName.end());

			m_spriteName.shrink_to_fit();
		}
	}
}