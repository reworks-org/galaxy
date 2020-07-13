///
/// AABB.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_AABB_HPP_
#define REDSHIFT_AABB_HPP_ 

#include <protostar/shapes/Rect.hpp>

///
/// Core namespace.
///
namespace rs
{
	///
	/// Test for AABB collision between rectangles.
	///
	/// \param a First rectangle.
	/// \param b Second rectangle.
	///
	/// \return True if there is (was) a collision.
	///
	const bool AABB(const pr::Rect<int>& a, const pr::Rect<int>& b) noexcept;
}

#endif