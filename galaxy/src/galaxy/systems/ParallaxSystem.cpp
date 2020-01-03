///
/// ParallaxSystem.cpp
/// galaxy
///
/// Created by reworks on 04/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/tags/CameraTag.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/components/EnabledComponent.hpp"
#include "galaxy/components/ParallaxComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"

#include "ParallaxSystem.hpp"

namespace galaxy
{
	void ParallaxSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		CameraTag& tag = Locator::world->m_registry.get<CameraTag>();

		// Calculate the new parallax positions using a "speed" multiplier and the current camera position.
		Locator::world->m_registry.view<ParallaxComponent, TransformComponent, EnabledComponent>()
			.each([&](entt::DefaultRegistry::entity_type entity, ParallaxComponent& pc, TransformComponent& tc, EnabledComponent& ec)
		{
			tc.m_rect.m_x += tag.m_bounds.m_x * pc.m_horizontalSpeed;
			tc.m_rect.m_y += tag.m_bounds.m_y * pc.m_verticalSpeed;
		});
	}
}