///
/// CameraSystem.cpp
/// starlight
///
/// Created by reworks on 05/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/tags/CameraTag.hpp"
#include "sl/events/EventTypes.hpp"
#include "sl/events/PlayerMoveEvent.hpp"

#include "CameraSystem.hpp"

namespace sl
{
	void CameraSystem::event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry)
	{
		switch (event->type)
		{
		case EventTypes::PLAYER_MOVE_EVENT:
			// When the player moves, update the camera's position to align with the new player position.
			PlayerMoveEvent* playerMoveEvent = (PlayerMoveEvent*)event->user.data1;
			CameraTag& camera = registry.get<CameraTag>();
			camera.update(playerMoveEvent->m_playerEntity);

			break;
		}
	}

	void CameraSystem::update(const double dt, entt::DefaultRegistry& registry)
	{
		// Only need to update camera when player moves.
	}
}