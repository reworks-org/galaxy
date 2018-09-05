///
/// ParallaxSystem.cpp
/// starlight
///
/// Created by reworks on 04/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/events/EventTypes.hpp"
#include "sl/events/PlayerMoveEvent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/ParallaxComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "ParallaxSystem.hpp"

namespace sl
{
	void ParallaxSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
		switch (event->type)
		{
			case EventTypes::PLAYER_MOVE_EVENT:
				PlayerMoveEvent* playerMoveEvent = (PlayerMoveEvent*)event->user.data1;
				
				// If a player moves, move the parallax background aswell, at the speed defined in the ParallaxComponent.
				registry.view<ParallaxComponent, TransformComponent, EnabledComponent>()
					.each([&](entt::DefaultRegistry::entity_type entity, ParallaxComponent& pc, TransformComponent& tc, EnabledComponent& ec)
				{
					tc.m_rect.m_x += playerMoveEvent->m_horizontalSpeed * pc.m_horizontalSpeed;
					tc.m_rect.m_y += playerMoveEvent->m_verticalSpeed * pc.m_verticalSpeed;
				});

				break;
		}
	}

	void ParallaxSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		// Parallax do not need to be updated for now. Only when a player moves.
	}
}