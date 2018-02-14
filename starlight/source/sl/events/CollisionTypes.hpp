///
/// CollisionTypes.hpp
/// starlight
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_COLLISIONTYPES_HPP_
#define STARLIGHT_COLLISIONTYPES_HPP_

#include <cinttypes>

namespace sl
{
	///
	/// \brief A data structure of collision types.
	///
	/// Can be inherited and you can add your own types.
	/// Don't override library types, which currently occupy 0x0001 - 0x0002.
	///
	struct CollisionTypes
	{
		static constexpr std::uint16_t WALL_COLLISION = 0x0001;
		static constexpr std::uint16_t HURT_TERRAIN_COLLISION = 0x0002;
	};

	typedef unsigned int CollisionType;
}

#endif