///
/// CollisionSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_COLLISIONSYSTEM_HPP_
#define GALAXY_SYSTEM_COLLISIONSYSTEM_HPP_

#include "galaxy/core/Scene2D.hpp"
#include "galaxy/math/QuadTree.hpp"
#include "galaxy/physics/DynamicTree.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Collision system.
		///
		class CollisionSystem final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			CollisionSystem() noexcept;

			///
			/// Destructor.
			///
			virtual ~CollisionSystem() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param scene Currently active scene.
			/// \param dt DeltaTime from gameloop.
			///
			void update(core::Scene2D* scene, const double dt) override;

		private:
			///
			/// Dynamic Tree for efficient collision detection.
			///
			physics::DynamicTree<ecs::Entity> m_bvh;

			///
			/// Offset from collision.
			///
			glm::vec2 m_mtv;

			///
			/// Possible entity memory cache.
			///
			std::vector<ecs::Entity> m_possible;

			///
			/// Quadtree for spacial partitioning.
			///
			math::Quadtree m_quadtree;

			///
			/// Output memory cache.
			///
			std::vector<math::Quadtree::Object*> m_output;
		};
	} // namespace systems
} // namespace galaxy

#endif