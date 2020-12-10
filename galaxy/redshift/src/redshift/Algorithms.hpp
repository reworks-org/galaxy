///
/// Algorithms.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_ALGORITHMS_HPP_
#define REDSHIFT_ALGORITHMS_HPP_

#include <protostar/graphics/Circle.hpp>

///
/// Core namespace.
///
namespace rs
{
	///
	/// Grouping for collision algorithms.
	///
	namespace collisions
	{
		///
		/// Check for collision between two circles.
		///
		/// \param a Object A.
		/// \param b Object B.
		///
		/// \return True if there is a collision.
		///
		[[nodiscard]] const bool circle_circle(const pr::Circle& a, const pr::Circle& b) noexcept;
	} // namespace collisions
} // namespace rs

#endif