//
//  Layer.cpp
//  rework
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/core/World.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/TextComponent.hpp"
#include "re/components/LayerComponent.hpp"
#include "re/components/SpriteComponent.hpp"
#include "re/components/TransformComponent.hpp"

#include "Layer.hpp"

namespace re
{
	Layer::Layer(TexturePacker* atlas)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::draw()
	{
		for (auto& sc : m_sprites)
		{
			m_atlas->al_draw_packed_bitmap(sc->m_spriteName, tc->m_x, tc->m_y, 0);
		}
	}

	void Layer::insert(ex::Entity& e)
	{
		m_entities.push_back(e);
	}

	void Layer::sort()
	{
		std::sort(m_sprites.begin(), m_sprites.end(), [](ex::Entity a, ex::Entity b) {
			
			return a.component<SpriteComponent>()->m_layer < b->m_layer;
		});

		std::sort(m_texts.begin(), m_texts.end(), [](ex::ComponentHandle<TextComponent> a, ex::ComponentHandle<TextComponent> b) {
			return a->m_layer < b->m_layer;
		});
	}
}