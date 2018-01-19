///
/// ParallaxSystem.cpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// Copyright (c) 2018+ reworks.
/// Refer to LICENSE.txt for more details.
///

#include "sl/components/ParallaxComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "ParallaxSystem.hpp"

namespace sl
{
	void ParallaxSystem::event(ALLEGRO_EVENT* event)
	{
	}

	void ParallaxSystem::update(const double dt, entt::DefaultRegistry& registery)
	{
		registery.view<ParallaxComponent>().each([this](entt::Entity entity, ParallaxComponent& pc)
		{
			
		});
	}
}