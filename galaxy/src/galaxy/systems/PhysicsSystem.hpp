///
/// PhysicsSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_PHYSICSSYSTEM_HPP_
#define GALAXY_SYSTEM_PHYSICSSYSTEM_HPP_

#include "galaxy/ecs/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// System that handles rendering of entities with a graphics::Sprite, TransformComponent, etc.
		///
		class PhysicsSystem final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			PhysicsSystem();

			///
			/// Argument constructor.
			///
			/// \param gravity Gravity to apply to this simulation.
			///
			PhysicsSystem(const glm::vec2& gravity);

			///
			/// Destructor.
			///
			virtual ~PhysicsSystem();

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param world Game World containing entities.
			/// \param dt "Lag" from gameloop.
			///
			void update(core::World& world, const double dt) override;
		};
	} // namespace systems
} // namespace galaxy

#endif