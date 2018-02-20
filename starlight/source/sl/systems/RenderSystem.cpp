///
/// RenderSystem.cpp
/// starlight
///
/// Created by reworks on 06/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/display.h>

#include "sl/core/World.hpp"
#include "sl/math/QuadTree.hpp"
#include "sl/mapping/Level.hpp"
#include "sl/tags/CameraTag.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/graphics/RenderType.hpp"
#include "sl/core/ServiceLocator.hpp"
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

	void RenderSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
	}

	void RenderSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		auto view = registry.view<RenderComponent>();

		m_entitys.clear(); 
		m_entitys.reserve(view.size());

		m_quadtree.updateBounds(Locator::m_world->m_currentLevel->getBounds());

		for (entt::Entity entity : view)
		{
			m_quadtree.insert(entity);
		}

		m_quadtree.retrieve(m_entitys, registry.get<CameraTag>().m_bounds);

		std::sort(m_entitys.begin(), m_entitys.end(), [&](entt::Entity a, entt::Entity b)
		{
			return registry.get<TransformComponent>(a).m_layer < registry.get<TransformComponent>(b).m_layer;
		});
	}

	void RenderSystem::render(entt::DefaultRegistry& registry)
	{
		al_hold_bitmap_drawing(true);
		
		for (entt::Entity entity : m_entitys)
		{
			auto& rc = registry.get<RenderComponent>(entity);
			for (auto& type : rc.m_renderTypes)
			{
				switch (type)
				{
					case RenderTypes::SPRITE:
					{
						auto sprtuple = registry.get<TransformComponent, SpriteComponent>(entity);
						auto& transformSprite = std::get<0>(sprtuple);
						auto& spriteSprite = std::get<1>(sprtuple);

						Locator::m_textureAtlas->al_draw_tinted_scaled_rotated_packed_bitmap(spriteSprite.m_spriteName, al_map_rgba_f(0.0f, 0.0f, 0.0f, spriteSprite.m_opacity), 0.0f, 0.0f, transformSprite.m_rect.m_x, transformSprite.m_rect.m_y, 0.0f, 0.0f, transformSprite.m_angle, 0);
					}
					break;

					case RenderTypes::TEXT:
					{
						auto textuple = registry.get<TransformComponent, TextComponent>(entity);
						auto& transformText = std::get<0>(textuple);
						auto& textText = std::get<1>(textuple);

						Locator::m_textureAtlas->al_draw_tinted_scaled_rotated_packed_bitmap(textText.m_id, al_map_rgba_f(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 0.0f, transformText.m_rect.m_x, transformText.m_rect.m_y, 0.0f, 0.0f, transformText.m_angle, 0);
					}
					break;

					case RenderTypes::PARTICLE:
					{
						auto partuple = registry.get<TransformComponent, ParticleComponent>(entity);
						auto& transformParticle = std::get<0>(partuple);
						auto& particle = std::get<1>(partuple);

						Locator::m_textureAtlas->al_draw_tinted_packed_bitmap(particle.m_id, al_map_rgba_f(0.0f, 0.0f, 0.0f, utils::customPercentage(particle.m_alpha, 0.0f, 255.0f)), transformParticle.m_rect.m_x, transformParticle.m_rect.m_y, 0);
					}
					break;

					default:
					{
						LOG_S(ERROR) << "Tried to render a type that is not renderable! Type (see RenderType.hpp): " << std::to_string(type);
					}
					break;
				}
			}
		}

		al_hold_bitmap_drawing(false);
	}
}