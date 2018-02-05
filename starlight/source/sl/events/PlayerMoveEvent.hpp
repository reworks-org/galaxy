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

struct PlayerMoveEvent
{
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
	/// We use the base type rather than the typedef to avoid includes.
	///
	unsigned int m_playerEntity;
};

#endif