///
/// AnimationSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_ANIMATIONSYSTEM_HPP_
#define GALAXY_SYSTEMS_ANIMATIONSYSTEM_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/ecs/System.hpp"
#include "galaxy/graphics/Camera.hpp"

namespace galaxy
{
	class core::World;

	namespace systems
	{
		///
		/// System that handles rendering of entities with a graphics::Sprite, TransformComponent, etc.
		///
		class AnimationSystem final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			AnimationSystem() noexcept;

			///
			/// Destructor.
			///
			virtual ~AnimationSystem() noexcept;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param world Game World containing entities.
			/// \param dt DeltaTime from gameloop.
			///
			void update(core::World& world, const double dt) override;
		};
	} // namespace systems
} // namespace galaxy

#endif