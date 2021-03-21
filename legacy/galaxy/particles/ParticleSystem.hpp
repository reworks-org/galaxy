///
/// ParticleSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEM_PARTICLESYSTEM_HPP_
#define GALAXY_SYSTEM_PARTICLESYSTEM_HPP_

#include "galaxy/ecs/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Handles transformations being applied to entities.
		///
		class ParticleSystem final : public ecs::System
		{
		public:
			///
			/// Constructor.
			///
			ParticleSystem() noexcept;

			///
			/// Destructor.
			///
			virtual ~ParticleSystem() noexcept;

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