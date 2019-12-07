///
/// CollisionTypes.hpp
/// galaxy
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COLLISIONTYPES_HPP_
#define GALAXY_COLLISIONTYPES_HPP_

#include <cstdint>

galaxy
{
	///
	/// \brief A data structure of collision types.
	///
	/// Can be inherited and you can add your own types.
	/// Don't override library types, which currently occupy 0x0001 - 0x0003.
	///
	struct CollisionTypes
	{
		///
		/// Representing a player collision event.
		///
		static constexpr std::uint16_t PLAYER_COLLISION = 0x0001;

		///
		/// Representing a wall collision event.
		///
		static constexpr std::uint16_t WALL_COLLISION = 0x0002;

		///
		/// Representing a damaging terrain type event.
		///
		static constexpr std::uint16_t HURT_TERRAIN_COLLISION = 0x0003;
	};

	///
	/// Allows the usage of CollisionType to make code more understandable.
	///
	typedef std::uint16_t CollisionType;
}

#endif