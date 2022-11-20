///
/// AnimationSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_ANIMATIONSYSTEM_HPP_
#define GALAXY_SYSTEMS_ANIMATIONSYSTEM_HPP_

#include "galaxy/systems/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Processess entities with a renderable component.
		///
		class AnimationSystem final : public systems::System
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
			/// \param layer Non-owning pointer to current layer.
			///
			void update(scene::Layer* layer) override;
		};
	} // namespace systems
} // namespace galaxy

#endif