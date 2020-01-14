///
/// CircleCircle.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_CIRCLECIRCLE_HPP_
#define REDSHIFT_CIRCLECIRCLE_HPP_ 

#include "redshift/shapes/Circle.hpp"
#include "redshift/algorithms/Result.hpp"

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
	const bool circleCircle(const rs::Circle& a, const rs::Circle& b);

	///
	/// \brief Collision detection algorithm between two circles.
	///
	/// Thanks to: https://stackoverflow.com/a/1736741.
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