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
		m_sprites.clear();
		m_texts.clear();

		m_sprites.reserve(20);
		m_texts.reserve(20);
	}

	Layer::~Layer()
	{
	}

	void Layer::draw()
	{
		for (auto& e : m_sprites)
		{
			m_atlas->al_draw_tinted_scaled_rotated_packed_bitmap(e.component<SpriteComponent>()->m_spriteName, al_map_rgba_f(1, 1, 1, 1), 0, 0, e.component<TransformComponent>()->m_x, e.component<TransformComponent>()->m_y, 1, 1, e.component<TransformComponent>()->m_angle, 0);
		}

		for (auto& e : m_texts)
		{
			e.component<TextComponent>()->draw();
		}
	}

	void Layer::insertSprite(ex::Entity& e)
	{
		m_sprites.push_back(e);
	}

	void Layer::insertText(ex::Entity& e)
	{
		m_texts.push_back(e);
	}

	/*
	void Layer::sort()
	{
		std::sort(m_sprites.begin(), m_sprites.end(), [](ex::Entity& a, ex::Entity& b)
		{
			return a.component<SpriteComponent>()->m_layer < b.component<SpriteComponent>()->m_layer;
		});

		std::sort(m_texts.begin(), m_texts.end(), [](ex::Entity& a, ex::Entity& b) 
		{
			return a.component<TextComponent>()->m_layer < b.component<TextComponent>()->m_layer;
		});

		m_sprites.shrink_to_fit();
		m_texts.shrink_to_fit();
	}
	*/

	void Layer::clean()
	{
		m_sprites.clear();
		m_texts.clear();

		m_sprites.reserve(20);
		m_texts.reserve(20);
	}
}