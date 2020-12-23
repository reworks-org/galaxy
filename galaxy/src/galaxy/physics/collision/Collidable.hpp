///
/// Collidable.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_COLLIDABLE_HPP_
#define REDSHIFT_COLLIDABLE_HPP_

#include "redshift/collision/AABB.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Interface to allow an object to collide with another object.
	///
	class Collidable
	{
	public:
		///
		/// Destructor.
		///
		virtual ~Collidable() = default;

		///
		/// Retrieve AABB bounds.
		///
		/// \return Reference to AABB box.
		///
		[[nodiscard]] AABB& get_aabb() noexcept;

	protected:
		///
		/// Constructor.
		///
		Collidable() = default;

		///
		/// Copy constructor.
		///
		Collidable(const Collidable&) = default;

		///
		/// Move constructor.
		///
		Collidable(Collidable&&) = default;

		///
		/// Copy assignment operator.
		///
		Collidable& operator=(const Collidable&) = default;

		///
		/// Move assignment operator.
		///
		Collidable& operator=(Collidable&&) = default;

		///
		/// AABB box.
		///
		AABB m_aabb;
	};
} // namespace rs

#endif