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
	/// \brief A point is just a Vector2 representing a point on the cartesian plane.
	///
	/// For integer points.
	///
	using iPoint = protostar::Vector2<int>;

	///
	/// \brief A point is just a Vector2 representing a point on the cartesian plane.
	///
	/// For float points.
	///
	using fPoint = protostar::Vector2<float>;
}

#endif