///
/// AABB.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_AABB_HPP_
#define REDSHIFT_AABB_HPP_

///
/// Core namespace.
///
namespace rs
{
	///
	/// Basic AABB bounding box data structure.
	///
	struct AABB
	{
		///
		/// Default constructor.
		///
		AABB() noexcept;

		///
		/// Argument constructor.
		///
		/// \param topLeft Represents the top left corner of a rectangle.
		/// \param bottomRight Represents the bottom right corner of a rectangle.
		///
		AABB(const double topLeft, const double bottomRight) noexcept;

		///
		/// Default destructor.
		///
		~AABB() = default;

		///
		/// Represents the top left corner of a rectangle.
		///
		double m_topLeft;

		///
		/// Represents the bottom right corner of a rectangle.
		///
		double m_bottomRight;
	};
}

#endif