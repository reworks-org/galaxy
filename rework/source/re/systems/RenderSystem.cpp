//
//  RenderSystem.cpp
//  rework
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <allegro5/display.h>

#include "re/components/RenderableComponent.hpp"

#include "RenderSystem.hpp"

namespace re
{
	RenderSystem::RenderSystem(unsigned int layers, unsigned int defaultAlloc, int quadtreeLayers, int quadtreeMaxObjects)
		:m_camera(nullptr), m_level(nullptr), m_layerCount(layers), m_defaultAlloc(defaultAlloc), m_quadtreeLayers(quadtreeLayers), m_quadtreeMaxObjects(quadtreeMaxObjects)
	{
		m_layers.clear();

		m_layers.reserve(m_layerCount);
		for (unsigned int i = 0; i < m_layerCount; ++i)
		{
			m_layers.emplace_back(m_defaultAlloc);
		}
	}

	RenderSystem::~RenderSystem()
	{
		m_layers.clear();
	}

	void RenderSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
	{
		// level bounds
		m_quadtree = new QuadTree(0, m_level->getBounds(), m_quadtreeLayers, m_quadtreeMaxObjects);

		es.each<RenderableComponent>([this](entityx::Entity& e, RenderableComponent& rc)
		{
			m_quadtree->insert(e);
		});

		clean();

		m_layers.reserve(m_layerCount);
		for (unsigned int i = 0; i < m_layerCount; ++i)
		{
			m_layers.emplace_back(m_defaultAlloc);
		}

		std::vector<entityx::Entity> e;
		m_quadtree->retrieve(e, m_camera->getBounds());

		for (auto& elem : e)
		{
			m_clf(elem);
		}

		m_quadtree->clear();
		delete m_quadtree;
	}

	void RenderSystem::render()
	{
		al_hold_bitmap_drawing(true);

		for (auto& l : m_layers)
		{
			l.render();
		}

		al_hold_bitmap_drawing(false);
	}

	void RenderSystem::clean()
	{
		for (auto& l : m_layers)
		{
			l.clean();
		}

		m_layers.clear();
	}

	void RenderSystem::setCamera(Camera* camera)
	{
		m_camera = camera;
	}

	void RenderSystem::setLevel(Level* level)
	{
		m_level = level;
	}
}