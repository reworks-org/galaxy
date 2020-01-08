///
/// CircleCircle.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_CIRCLECIRCLE_HPP_
#define REDSHIFT_CIRCLECIRCLE_HPP_ 

#include "redshift/Types.hpp"
#include "redshift/shapes/Circle.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Collision detection algorithm between two circles.
	///
	/// \param a First circle to check.
	/// \param b Second circle to check.
	///
	/// \return Boolean True if there is a collison between a and b.
	///
	const bool circleCircle(const rs::Circle& a, const rs::Circle& b);

	///
	/// Collision detection algorithm between two circles.
	///
	/// \param c1 Center of circle A.
	/// \param r1 Radius of circle A.
	/// \param c2 Center of circle B.
	/// \param r2 Radius of circle B.
	///
	/// \return Boolean True if there is a collison between a and b.
	///
	const bool circleCircle(const rs::Point& c1, const std::uint32_t r1, const rs::Point& c2, const std::uint32_t r2);
}

#endif