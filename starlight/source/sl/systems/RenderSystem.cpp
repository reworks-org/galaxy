///
/// RenderSystem.cpp
/// starlight
///
///  Created by reworks on 06/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <allegro5/display.h>

#include "loguru/loguru.hpp"
#include "sl/math/QuadTree.hpp"
#include "sl/tags/LevelTag.hpp"
#include "sl/tags/CameraTag.hpp"
#include "sl/graphics/RenderType.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/TransformComponent.hpp"

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
		auto view = registery.view<RenderComponent>();

		m_entitys.clear();
		m_entitys.reserve(view.size());

		LevelTag& level = registery.get<LevelTag>();
		m_quadtree.updateBounds(level.getBounds());

		view.each([this](entt::Entity e, RenderComponent& rc)
		{
			m_quadtree.insert(e);
		});

		m_quadtree.retrieve(m_entitys, registery.get<CameraTag>().getBounds());

		std::sort(m_entitys.begin(), m_entitys.end(), [&](entt::Entity a, entt::Entity b)
		{
			return registery.get<TransformComponent>(a).m_layer < registery.get<TransformComponent>(b).m_layer;
		});
	}

	void RenderSystem::render(entt::DefaultRegistry& registery)
	{
		al_hold_bitmap_drawing(true);

		for (entt::Entity entity : m_entitys)
		{
			auto& rc = registery.get<RenderComponent>(entity);
			for (auto& type : rc.m_renderTypes)
			{
				switch (type)
				{
				case RenderTypes::SPRITE:
					break;

				case RenderTypes::TEXT:
					break;

				case RenderTypes::PARALLAX:
					break;

				case RenderTypes::PARTICLE:
					break;

				case RenderTypes::MAP:
					break;

				default:
					LOG_S(ERROR) << "Tried to render a type that is not renderable! Type (see RenderType.hpp): " << std::to_string(type);
					break;
				}
			}
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