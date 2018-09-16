///
/// ScrollingBackgroundSystem.cpp
/// starlight
///
/// Created by reworks on 27/07/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/TransformComponent.hpp"
#include "sl/components/ScrollingBackgroundComponent.hpp"

#include "ScrollingBackgroundSystem.hpp"

namespace sl
{
	void ScrollingBackgroundSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		// For each entity with a scrolling background.
		registry.view<TransformComponent, RenderComponent, ScrollingBackgroundComponent, EnabledComponent>()
			.each([&](entt::DefaultRegistry::entity_type entity, TransformComponent& tc, RenderComponent& rc, ScrollingBackgroundComponent& sc, EnabledComponent& ec)
		{
			// Correct for boundaries.
			if (tc.m_rect.m_x < -1279)
			{
				tc.m_rect.m_x = 0;
			}

			// Update secondary render component.
			RenderComponent& secondRC = Locator::world->m_registry.get<RenderComponent>(sc.m_secondEntity);
			secondRC.m_opacity = rc.m_opacity;
			secondRC.m_textureName = rc.m_textureName;

			// Update secondary transform component.
			TransformComponent& secondTC = Locator::world->m_registry.get<TransformComponent>(sc.m_secondEntity);
			secondTC.m_angle = tc.m_angle;
			secondTC.m_layer = tc.m_layer;
			
			// Calculate transform area that is currently offscreen and set the new transform to make that offset onscreen.
			secondTC.m_rect.m_x = Locator::window->getSize().m_x + (tc.m_rect.m_x - 1);
			secondTC.m_rect.m_y = tc.m_rect.m_y;
			secondTC.m_rect.m_width = (tc.m_rect.m_x - 0) * -1;
			secondTC.m_rect.m_height = tc.m_rect.m_height;

			// Then scroll screen.
			tc.m_rect.m_x -= sc.m_speed;
		});
	}
}