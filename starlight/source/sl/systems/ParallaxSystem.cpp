///
/// ParallaxSystem.cpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// Copyright (c) 2018+ reworks.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/events.h>

#include "sl/events/Keys.hpp"
#include "sl/utils/ConfigReader.hpp"
#include "sl/components/ParallaxComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "ParallaxSystem.hpp"

namespace sl
{
	void ParallaxSystem::event(ALLEGRO_EVENT* event)
	{
		switch (event->type)
		{
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (event->keyboard.keycode)
			{
			case Keys::KEY_LEFT:
				break;

			case Keys::KEY_RIGHT:
				break;
			}
			break;
		}
	}

	void ParallaxSystem::update(const double dt, entt::DefaultRegistry& registery)
	{
		registery.view<ParallaxComponent>().each([this](entt::Entity entity, ParallaxComponent& pc)
		{
			
		});
	}
}