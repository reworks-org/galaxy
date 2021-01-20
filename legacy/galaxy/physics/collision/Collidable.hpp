///
/// Collidable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_COLLISION_COLLIDABLE_HPP_
#define GALAXY_PHYSICS_COLLISION_COLLIDABLE_HPP_

#include "galaxy/physics/collision/AABB.hpp"

namespace galaxy
{
	namespace physics
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
			virtual ~Collidable() noexcept = default;

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
			Collidable() noexcept = default;

			///
			/// AABB box.
			///
			AABB m_aabb;
		};
	} // namespace physics
} // namespace galaxy

#endif