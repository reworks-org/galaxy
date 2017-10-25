//
//  RenderSystem.cpp
//  rework
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/components/SpriteComponent.hpp"

#include "RenderSystem.hpp"

namespace re
{
	RenderSystem::RenderSystem(unsigned int layers, unsigned int defaultAlloc)
		:m_layerCount(layers), m_defaultAlloc(defaultAlloc)
	{
		m_layers.resize(0);
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

	void RenderSystem::update(ex::EntityManager& es, ex::EventManager& events, ex::TimeDelta dt)
	{
		clean();

		m_layers.resize(0);
		m_layers.reserve(m_layerCount);
		for (unsigned int i = 0; i < m_layerCount; ++i)
		{
			m_layers.emplace_back(m_defaultAlloc);
		}
	}

	void RenderSystem::render()
	{
		for (auto& l : m_layers)
		{
			l.render();
		}
	}

	void RenderSystem::clean()
	{
		for (auto& l : m_layers)
		{
			l.clean();
		}

		m_layers.clear();
	}
}
