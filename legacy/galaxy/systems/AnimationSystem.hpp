///
/// AnimationSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_ANIMATIONSYSTEM_HPP_
#define GALAXY_SYSTEMS_ANIMATIONSYSTEM_HPP_

#include "galaxy/core/Scene2D.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// System that handles animating entities.
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
			/// \param scene Currently active scene.
			///
			void update(core::Scene2D* scene) override;
		};
	} // namespace systems
} // namespace galaxy

#endif