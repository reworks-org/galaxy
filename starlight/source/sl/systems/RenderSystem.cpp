///
/// RenderSystem.cpp
/// starlight
///
/// Created by reworks on 06/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/display.h>

#include "sl/tags/CameraTag.hpp"
#include "sl/core/StateMachine.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "RenderSystem.hpp"

namespace sl
{
	RenderSystem::RenderSystem(int quadTreeLevels, int quadTreeMaxObjects)
		:m_quadTreeLevels(quadTreeLevels), m_quadTreeMaxObjects(quadTreeMaxObjects)
	{
	}

	RenderSystem::~RenderSystem()
	{
		m_quadtree.reset();
		m_entitys.clear();
	}

	void RenderSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
	}

	void RenderSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		auto view = registry.view<RenderComponent, TransformComponent>();
		m_quadtree = std::make_unique<QuadTree>(0, Rect<float, int>{ 0.0f, 0.0f, 0, 0 }, m_quadTreeLevels, m_quadTreeMaxObjects);
		
		#ifdef NDEBUG
			m_quadtree->updateBounds(Locator::stateMachine->top()->m_bounds);
		#else
			if (Locator::stateMachine->top() != nullptr)
			{
				m_quadtree->updateBounds(Locator::stateMachine->top()->m_bounds);
			}
			else
			{
				m_quadtree->updateBounds(Rect<float, int>{ 0.0f, 0.0f, 0, 0 });
			}
		#endif

		m_entitys.clear(); 
		m_entitys.reserve(view.size());

		for (entt::DefaultRegistry::entity_type entity : view)
		{
			m_quadtree->insert(entity);
		}
		
		m_quadtree->retrieve(m_entitys, registry.get<CameraTag>().m_bounds);

		std::sort(m_entitys.begin(), m_entitys.end(), [&](entt::DefaultRegistry::entity_type a, entt::DefaultRegistry::entity_type b)
		{
			return registry.get<TransformComponent>(a).m_layer < registry.get<TransformComponent>(b).m_layer;
		});

		m_quadtree.reset();
	}

	void RenderSystem::render(entt::DefaultRegistry& registry)
	{
		al_hold_bitmap_drawing(true);

		for (entt::DefaultRegistry::entity_type entity : m_entitys)
		{
			auto& rc = registry.get<RenderComponent>(entity);
			auto& tc = registry.get<TransformComponent>(entity);

			Locator::textureAtlas->al_draw_tinted_scaled_rotated_packed_bitmap(rc.m_textureName, al_map_rgba_f(1.0f, 1.0f, 1.0f, rc.m_opacity), 0.0f, 0.0f, tc.m_rect.m_x, tc.m_rect.m_y, 1.0f, 1.0f, tc.m_angle, 0);
		}

		al_hold_bitmap_drawing(false);
	}
}