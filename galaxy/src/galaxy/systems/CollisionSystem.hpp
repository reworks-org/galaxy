///
/// CollisionSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_COLLISIONSYSTEM_HPP_
#define GALAXY_SYSTEM_COLLISIONSYSTEM_HPP_

#include "galaxy/ecs/Entity.hpp"
#include "galaxy/ecs/System.hpp"
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
			virtual ~CollisionSystem() noexcept = default;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param world Game World containing entities.
			/// \param dt "Lag" from gameloop.
			///
			void update(core::World& world, const double dt) override;

			///
			/// Retrieve dynamic AABB tree.
			///
			/// \return Pointer to dynamic tree.
			///
			[[nodiscard]] physics::DynamicTree<ecs::Entity>* get_tree() noexcept;

		private:
			///
			/// Dynamic Tree for efficient collision detection.
			///
			physics::DynamicTree<ecs::Entity> m_tree;

			///
			/// Offset from collision.
			///
			glm::vec2 m_mtv;
		};
	} // namespace systems
} // namespace galaxy

#endif