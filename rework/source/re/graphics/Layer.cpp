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
		Locator::get<World>()->m_entityManager.each<SpriteComponent, TransformComponent>([](Entity entity, SpriteComponent& sc, TransformComponent& tc))
		{
			m_atlas->al_draw_packed_bitmap(sc->m_spriteName, tc->m_x, tc->m_y, 0);
		};)
	}
}