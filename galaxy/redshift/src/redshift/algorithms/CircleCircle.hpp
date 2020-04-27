///
/// CircleCircle.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_CIRCLECIRCLE_HPP_
#define REDSHIFT_CIRCLECIRCLE_HPP_ 

#include <protostar/math/Point.hpp>
#include <protostar/shapes/Circle.hpp>

///
/// Core namespace.
///
namespace rs
{
	///
	/// \brief Collision detection algorithm between two circles.
	///
	/// Thanks to: https://stackoverflow.com/a/1736741.
	///
	/// \param a First circle to check.
	/// \param b Second circle to check.
	///
	/// \return Boolean True if there is a collison between a and b.
	///
	const bool circleCircle(const protostar::Circle<int>& a, const protostar::Circle<int>& b);
}

#endif