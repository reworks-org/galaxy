///
/// RenderSystem.cpp
/// starlight
///
/// Created by reworks on 06/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/tags/CameraTag.hpp"
#include "sl/core/StateMachine.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "RenderSystem.hpp"

namespace sl
{
	RenderSystem::RenderSystem(int quadTreeLevels, int quadTreeMaxObjects)
		:m_quadTreeLevels(quadTreeLevels), m_quadTreeMaxObjects(quadTreeMaxObjects)
	{
		// Argument constructor.
	}

	RenderSystem::~RenderSystem()
	{
		// Ensure quadtree is properly destroyed.
		m_quadtree.reset();
		m_entitys.clear();
	}

	void RenderSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		// Retrieve the current entities to render and reset the quadtree.
		auto view = registry.view<RenderComponent, TransformComponent, EnabledComponent>();
		m_quadtree = std::make_unique<QuadTree>(0, Locator::stateMachine->top()->m_bounds, m_quadTreeLevels, m_quadTreeMaxObjects);
		
		// Prepare entity vector.
		m_entitys.clear(); 
		m_entitys.reserve(view.size());

		// Insert entities to process with quadtree.
		for (entt::DefaultRegistry::entity_type entity : view)
		{
			m_quadtree->insert(entity);
		}
		
		// Retrieve entities that are within the camera bounds.
		m_quadtree->retrieve(m_entitys, registry.get<CameraTag>().m_bounds);
		
		// Sort those entities for rendering.
		std::sort(m_entitys.begin(), m_entitys.end(), [&](entt::DefaultRegistry::entity_type a, entt::DefaultRegistry::entity_type b)
		{
			return registry.get<TransformComponent>(a).m_layer < registry.get<TransformComponent>(b).m_layer;
		});

		// Destroy quadtree to ensure memory quadtree is using is freed.
		m_quadtree.reset();
	}

	void RenderSystem::render()
	{
		// This optimises rendering performance when rendering from 1 texture i.e. the texture atlas.
		al_hold_bitmap_drawing(true);

		// Render each entity in the vector.
		for (entt::DefaultRegistry::entity_type entity : m_entitys)
		{
			auto& rc = Locator::world->m_registry.get<RenderComponent>(entity);
			auto& tc = Locator::world->m_registry.get<TransformComponent>(entity);

			// Using the texture atlas, so texture data does not have to be changed.
			Locator::textureAtlas->al_draw_tinted_scaled_rotated_packed_bitmap(rc.m_textureName, al_map_rgba_f(1.0f, 1.0f, 1.0f, rc.m_opacity), 0.0f, 0.0f, tc.m_rect.m_x, tc.m_rect.m_y, 1.0f, 1.0f, tc.m_angle, 0);
		}

		al_hold_bitmap_drawing(false);
	}
}