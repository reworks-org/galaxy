///
/// PlayerMoveEvent.hpp
/// starlight
///
/// Created by reworks on 04/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PLAYERMOVEEVENT_HPP_
#define STARLIGHT_PLAYERMOVEEVENT_HPP_

#include <cinttypes>

namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	struct PlayerMoveEvent
	{
		///
		/// Constructor.
		///
		inline PlayerMoveEvent(float horizontalSpeed, float verticalSpeed, entt::Entity playerEntity)
			:m_horizontalSpeed(horizontalSpeed), m_verticalSpeed(verticalSpeed), m_playerEntity(playerEntity)
		{
		}

		///
		/// How much to move the player by horizontally.
		///
		float m_horizontalSpeed;

		///
		/// How much to move the player by vertically.
		///
		float m_verticalSpeed;

		///
		/// The player entity.
		///
		entt::Entity m_playerEntity;
	};
}

#endif