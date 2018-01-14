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
#include "sl/core/World.hpp"
#include "sl/math/QuadTree.hpp"
#include "sl/mapping/Level.hpp"
#include "sl/tags/CameraTag.hpp"
#include "sl/graphics/RenderType.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/TextComponent.hpp"
#include "sl/components/SpriteComponent.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "RenderSystem.hpp"

namespace sl
{
	RenderSystem::RenderSystem(int quadTreeLevels, int quadtreeMaxObjects)
	:m_quadtree(0, {0.0f, 0.0f, 0, 0}, quadTreeLevels, quadtreeMaxObjects)
	{
	}

	RenderSystem::~RenderSystem()
	{
		m_entitys.clear();
	}

	void RenderSystem::update(const double dt, entt::DefaultRegistry& registery)
	{
		auto view = registery.view<RenderComponent>();

		m_entitys.clear();
		m_entitys.reserve(view.size());

		m_quadtree.updateBounds(World::get()->m_currentLevel->getBounds());

		view.each([this](entt::Entity e, RenderComponent& rc)
		{
			m_quadtree.insert(e);
		});

		m_quadtree.retrieve(m_entitys, registery.get<CameraTag>().m_bounds);

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
					auto sprtuple = registery.get<TransformComponent, SpriteComponent>(entity);
					TextureAtlas::get()->al_draw_packed_bitmap(std::get<1>(sprtuple).m_spriteName, std::get<0>(sprtuple).m_rect.m_x, std::get<0>(sprtuple).m_rect.m_y, 0);
					break;

				case RenderTypes::TEXT:
					auto textuple = registery.get<TransformComponent, TextComponent>(entity);
					TextureAtlas::get()->al_draw_packed_bitmap(std::get<1>(textuple).m_id, std::get<0>(textuple).m_rect.m_x, std::get<0>(textuple).m_rect.m_y, 0);
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
}