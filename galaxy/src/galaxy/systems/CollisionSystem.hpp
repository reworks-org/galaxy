///
/// CollisionSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_COLLISIONSYSTEM_HPP_
#define GALAXY_SYSTEM_COLLISIONSYSTEM_HPP_

#include "galaxy/core/Scene2D.hpp"
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
			///
			void update(core::Scene2D* scene) override;

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
		};
	} // namespace systems
} // namespace galaxy

#endif