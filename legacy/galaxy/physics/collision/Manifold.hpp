///
/// Manifold.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_COLLISION_MANIFOLD_HPP_
#define GALAXY_PHYSICS_COLLISION_MANIFOLD_HPP_

#include <glm/vec2.hpp>

namespace galaxy
{
	namespace physics
	{
		///
		/// Holds data relating to a collision.
		///
		struct Manifold final
		{
			///
			/// Constructor.
			///
			Manifold() noexcept;

			///
			/// Destructor.
			///
			~Manifold() noexcept = default;

			///
			/// Penetration by A into B's AABB.
			///
			float m_penetration;

			///
			/// The collision direction.
			///
			glm::vec2 m_normal;
		};
	} // namespace physics
} // namespace galaxy

#endif