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

		private:
			///
			/// Dynamic Tree for efficient collision detection.
			///
			physics::DynamicTree<ecs::Entity> m_tree;

			///
			/// Dispatcher pointer.
			///
			events::Dispatcher* m_dispatcher;

			///
			/// Offset from collision.
			///
			glm::vec2 m_mtv;
		};
	} // namespace systems
} // namespace galaxy

#endif