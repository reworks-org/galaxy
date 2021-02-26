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
#include "galaxy/events/dispatcher/Dispatcher.hpp"

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
			/// Argument constructor.
			///
			/// \param dispatcher Pointer to dispatcher to trigger collision events from.
			///
			CollisionSystem(events::Dispatcher* dispatcher);

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

		private:
			///
			/// Default constructor.
			///
			CollisionSystem() = delete;

			///
			/// \brief Narrow phase collision.
			///
			/// Uses SAT (seperating axis theorm).
			///
			/// \param world Game World containing entities.
			/// \param a First entity that is colliding.
			/// \param b Second entity that is colliding.
			/// \param mtv Minimum transation vector used to resolve the collision.
			///
			/// \return True if there is a collision.
			///
			[[nodiscard]] const bool narrow_phase_collision(core::World& world, const ecs::Entity a, const ecs::Entity b, glm::vec2& mtv);

		private:
			///
			/// Dynamic Tree for efficient collision detection.
			///
			physics::DynamicTree<ecs::Entity> m_tree;

			///
			/// Dispatcher pointer.
			///
			events::Dispatcher* m_dispatcher;
		};
	} // namespace systems
} // namespace galaxy

#endif