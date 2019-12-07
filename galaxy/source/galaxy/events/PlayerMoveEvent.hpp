///
/// PlayerMoveEvent.hpp
/// galaxy
///
/// Created by reworks on 04/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLAYERMOVEEVENT_HPP_
#define GALAXY_PLAYERMOVEEVENT_HPP_

#include "galaxy/libs/entt/entity/registry.hpp"

namespace galaxy
{
	///
	/// A player move event used by the library.
	///
	struct PlayerMoveEvent final
	{
		///
		/// Constructor.
		///
		inline PlayerMoveEvent(float horizontalSpeed, float verticalSpeed, entt::DefaultRegistry::entity_type playerEntity) noexcept
			:m_horizontalSpeed(horizontalSpeed), m_verticalSpeed(verticalSpeed), m_playerEntity(playerEntity)
		{
		}

		///
		/// How much to move the player by horizontally.
		///
		float m_horizontalSpeed = 0.0f;

		///
		/// How much to move the player by vertically.
		///
		float m_verticalSpeed = 0.0f;

		///
		/// The player entity.
		///
		entt::DefaultRegistry::entity_type m_playerEntity = 0;
	};
}

#endif