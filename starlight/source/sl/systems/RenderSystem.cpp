///
/// RenderSystem.cpp
/// starlight
///
///  Created by reworks on 06/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "sl/math/QuadTree.hpp"
#include "sl/components/TransformComponent.hpp"
#include "sl/components/RenderableComponent.hpp"

#include "RenderSystem.hpp"

namespace sl
{
	RenderSystem::RenderSystem(unsigned int layers, unsigned int defaultAlloc, int quadTreeLevels, int quadtreeMaxObjects)
		:m_layerCount(layers), m_defaultAlloc(defaultAlloc), m_quadtree(0, {0.0f, 0.0f, 0, 0}, quadTreeLevels, quadtreeMaxObjects)
	{
		allocLayers();
	}

	RenderSystem::~RenderSystem()
	{
		m_layers.clear();
		m_entitys.clear();
	}

	void RenderSystem::update(const double dt, entt::DefaultRegistry& registery)
	{
		auto view = registery.view<RenderableComponent>();
		
		LevelTag& level = registery.get<LevelTag>();
		m_quadtree.updateBounds(level.getBounds());

		view.each([this](entt::Entity e, RenderableComponent& rc)
		{
			m_quadtree.insert(e);
		});




		/*
		clean();
		allocLayers();

		std::vector<entityx::Entity> e;
		
		if (m_camera != nullptr)
		{
			m_quadtree->retrieve(e, m_camera->getBounds());
		}
		else
		{
			m_quadtree->retrieve(e, {0, 0, Locator::get<Window>()->getSize().x, Locator::get<Window>()->getSize().y});
		}

		for (auto& elem : e)
		{
			m_clf(elem);
		}

		m_quadtree->clear();
		delete m_quadtree;
		*/
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

	unsigned int RenderSystem::getRenderingLayers() const
	{
		return m_layerCount;
	}

	void RenderSystem::allocLayers()
	{
		m_layers.reserve(m_layerCount);
		for (unsigned int i = 0; i < m_layerCount; ++i)
		{
			m_layers.emplace_back(m_defaultAlloc);
		}
	}
}