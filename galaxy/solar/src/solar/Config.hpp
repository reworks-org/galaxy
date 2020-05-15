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
/// Library wide default integer.
/// Must be 32bits. Do not change.
///
#define SR_INTEGER std::uint32_t

///
/// Core namespace.
///
namespace sr
{
	///
	/// Entity type.
	/// Must be 32bits. Do not change.
	///
	using Entity = SR_INTEGER;

	///
	/// DeltaTime type.
	///
	using DeltaTime = double;

	///
	/// BitMask to see if entity is valid.
	///
	static constexpr inline unsigned int VALID_ENTITY = 1;
}

#endif