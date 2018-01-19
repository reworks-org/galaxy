///
/// ParallaxSystem.hpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// Copyright (c) 2018+ reworks.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARALLAXSYSTEM_HPP_
#define STARLIGHT_PARALLAXSYSTEM_HPP_

#include <vector>

#include "sl/types/System.hpp"

namespace sl
{
	class ParallaxSystem : public System
	{
	public:
		///
		/// Default Constructor.
		///
		ParallaxSystem() = default;

		///
		/// Destructor.
		///
		~ParallaxSystem() override = default;

		///
		///
		///
		template<typename Tag>
		void registerPlayerTag(entt::DefaultRegistry& registery);

	private:
		std::function<entt::Entity(void)> m_getPlayerTag;

	private:
		///
		/// Lets systems recieve events.
		///
		/// \param event ALLEGRO_EVENT passed by application class.
		///
		void event(ALLEGRO_EVENT* event) override;

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		void update(const double dt, entt::DefaultRegistry& registery) override;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ParallaxSystem(const ParallaxSystem&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ParallaxSystem(ParallaxSystem&&) = delete;
	};

	template<typename Tag>
	void ParallaxSystem::registerPlayerTag(entt::DefaultRegistry& registery)
	{
		registery.get<Tag>();
	}
}

#endif