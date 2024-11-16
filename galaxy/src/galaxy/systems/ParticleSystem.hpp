///
/// ParticleSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SYSTEMS_PARTICLESYSTEM_HPP_
#define GALAXY_SYSTEMS_PARTICLESYSTEM_HPP_

#include "galaxy/systems/System.hpp"

namespace galaxy
{
	namespace systems
	{
		///
		/// Update particle positions.
		///
		class ParticleSystem final : public systems::System
		{
		  public:
			///
			/// Constructor.
			///
			ParticleSystem();

			///
			/// Destructor.
			///
			virtual ~ParticleSystem();

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param scene Non-owning pointer to current scene.
			///
			void update(scene::Scene* scene) override;

		  private:
			///
			/// Forces being applied to the particles.
			///
			glm::vec2 m_forces;
		};
	} // namespace systems
} // namespace galaxy

#endif
