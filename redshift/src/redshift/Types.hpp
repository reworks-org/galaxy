///
/// Types.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_TYPES_HPP_
#define REDSHIFT_TYPES_HPP_

#include <cstdint>

#include <protostar/math/Vector2.hpp>

///
/// Core namespace.
///
namespace rs
{
	///
	/// Shortcut for using protostar::Vector2.
	///
	using Point = protostar::Vector2<std::uint32_t>;
}

#endif