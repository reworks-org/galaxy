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
#include "sl/mapping/Level.hpp"
#include "sl/tags/CameraTag.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/RenderComponent.hpp"
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

		//m_quadtree.updateBounds(Locator::world->m_currentLevel->getBounds());

		for (entt::Entity entity : view)
		{
			//m_quadtree.insert(entity);
			m_entitys.push_back(entity);
		}

		//m_quadtree.retrieve(m_entitys, registry.get<CameraTag>().m_bounds);

		std::sort(m_entitys.begin(), m_entitys.end(), [&](entt::Entity a, entt::Entity b)
		{
			return registry.get<TransformComponent>(a).m_layer < registry.get<TransformComponent>(b).m_layer;
		});
	}

	void RenderSystem::render(entt::DefaultRegistry& registry)
	{
		al_hold_bitmap_drawing(true);

		// m_entitys
		for (entt::Entity& entity : m_entitys)
		{
			auto& rc = registry.get<RenderComponent>(entity);
			auto& tc = registry.get<TransformComponent>(entity);

			Locator::textureAtlas->al_draw_tinted_scaled_rotated_packed_bitmap(rc.m_textureName, al_map_rgba_f(1.0f, 1.0f, 1.0f, rc.m_opacity), 0.0f, 0.0f, tc.m_rect.m_x, tc.m_rect.m_y, 1.0f, 1.0f, tc.m_angle, 0);
		}

		al_hold_bitmap_drawing(false);
	}
}