///
/// ParticleSystem.hpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARTICLESYSTEM_HPP_
#define STARLIGHT_PARTICLESYSTEM_HPP_

#include "sl/types/System.hpp"
#include "sl/events/EmitParticleEvent.hpp"

namespace sl
{
	class World;

	///
	/// Process's entities with ParticleComponents.
	///
	class ParticleSystem final : public System
	{
	public:
		///
		/// Default Constructor.
		///
		ParticleSystem();

		///
		/// Destructor.
		///
		~ParticleSystem() override = default;

		///
		/// Function to connect to the event dispatcher.
		///
		/// \param pe ParticleSystem will recieve emit particle events.
		///
		void receive(const EmitParticleEvent& pe);

	private:
		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		/// \param dt Delta Time from update loop.
		/// \param registry Default entity registry.
		///
		void update(const double dt, entt::DefaultRegistry& registry) override;
	};
}

#endif