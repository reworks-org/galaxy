///
/// Point.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_POINT_HPP_
#define PROTOSTAR_POINT_HPP_

#include "protostar/math/Vector2.hpp"

///
/// Core namespace.
///
namespace protostar
{
	///
	/// A point is just a vector2 representing a point on the cartesian plane.
	///
	using Point = protostar::Vector2<int>;
}

#endif