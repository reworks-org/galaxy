///
/// Config.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_CONFIG_HPP_
#define SOLAR_CONFIG_HPP_

#include <cstdint>

///
/// Legacy.
/// Must be 32bits. Do not change.
///
#define SR_INTEGER std::uint32_t

namespace sr
{
	///
	/// Entity type.
	/// Must be 32bits. Do not change.
	///
	using Entity = SR_INTEGER;

	///
	/// Event type.
	///
	using Event = int;

	///
	/// DeltaTime type.
	///
	using DeltaTime = double;

	///
	/// BitMask to see if entity is valid.
	///
	static const inline unsigned int VALID_ENTITY = 1;
}

#endif