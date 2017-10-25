//
//  SpriteComponent.cpp
//  rework
//
//  Created by reworks on 10/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <algorithm>

#include "entityx/Entity.h"
#include "imgui/imgui_impl_a5.h"
#include "re/graphics/TextureAtlas.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/TransformComponent.hpp"

#include "SpriteComponent.hpp"

namespace re
{
	SpriteComponent::SpriteComponent(ex::Entity& e, sol::table& table)
	{
		m_spriteName = table.get<std::string>("spriteName");
		m_layer = table.get<int>("layer");
		m_entity = e;
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::debug()
	{
		ImGui::stl::InputText("Sprite Name (in atlas):", &m_spriteName, ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::Spacing();
		if (ImGui::InputInt("Layer: ", &m_layer, 1, 2))
		{
			if (m_layer < 0)
				m_layer = 0;
		}
	}

	void SpriteComponent::render()
	{
		Locator::get<TextureAtlas>()->al_draw_tinted_scaled_rotated_packed_bitmap(m_entity.component<SpriteComponent>()->m_spriteName, al_map_rgba_f(1, 1, 1, 1), 0, 0, m_entity.component<TransformComponent>()->m_x, m_entity.component<TransformComponent>()->m_y, 1, 1, m_entity.component<TransformComponent>()->m_angle, 0);
	}
}