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
		ParticleSystem() = default;

		///
		/// Destructor.
		///
		~ParticleSystem() override = default;

	private:
		///
		/// Lets systems recieve events.
		///
		/// \param event ALLEGRO_EVENT passed by application class.
		///
		void event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry) override;

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		void update(const double dt, entt::DefaultRegistry& registry) override;
	};
}

#endif