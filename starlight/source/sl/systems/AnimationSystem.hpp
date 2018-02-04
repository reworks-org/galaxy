///
/// AnimationSystem.hpp
/// starlight
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_ANIMATIONSYSTEM_HPP_
#define STARLIGHT_ANIMATIONSYSTEM_HPP_

#include "sl/types/System.hpp"

namespace sl
{
	class AnimationSystem final : public System
	{
	public:
		///
		/// Constructor.
		///
		AnimationSystem() = default;

		///
		/// Destructor
		///
		~AnimationSystem() override = default;

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