///
/// Manifold.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_MANIFOLD_HPP_
#define REDSHIFT_MANIFOLD_HPP_

#include <glm/vec2.hpp>

///
/// Core namespace.
///
namespace rs
{
	///
	/// Holds data relating to a collision.
	///
	struct Manifold final
	{
		///
		/// Constructor.
		///
		Manifold();

		///
		/// Copy constructor.
		///
		Manifold(const Manifold&) = default;

		///
		/// Move constructor.
		///
		Manifold(Manifold&&) = default;

		///
		/// Copy assignment operator.
		///
		Manifold& operator=(const Manifold&) = default;

		///
		/// Move assignment operator.
		///
		Manifold& operator=(Manifold&&) = default;

		///
		/// Destructor.
		///
		~Manifold() = default;

		///
		/// Penetration by A into B's AABB.
		///
		float m_penetration;

		///
		/// The collision direction.
		///
		glm::vec2 m_normal;
	};
} // namespace rs

#endif