//
//  Test.cpp
//  Sandbox
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <allegro5/events.h>
#include <sl/core/World.hpp>
#include <sl/utils/Time.hpp>
#include <loguru/loguru.hpp>
#include <sl/graphics/Window.hpp>
#include <sl/systems/RenderSystem.hpp>
#include <sl/managers/AudioManager.hpp>
#include <sl/services/ServiceLocator.hpp>

#include "gamesystems/MoveSystem.hpp"

#include "Test.hpp"

using namespace sl;

namespace moving
{
	static bool north = false;
	static bool south = false;
	static bool east = false;
	static bool west = false;
}

Test::~Test()
{
}

void Test::load()
{
	//Locator::get<World>()->createEntity("test.lua");
	
	if (!m_doneOnce)
	{
		Locator::get<World>()->createEntities("world.lua");
		m_doneOnce = true;
	}

	Locator::get<AudioManager>()->getMusic("background")->play();
}

void Test::unload()
{
	Locator::get<AudioManager>()->getMusic("background")->stop();
	Locator::get<World>()->m_systemManager.system<RenderSystem>()->clean();
}

void Test::event(ALLEGRO_EVENT* event)
{
	switch (event->type)
	{
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (event->keyboard.keycode)
		{
			case ALLEGRO_KEY_ESCAPE:
				Locator::get<Window>()->close();
				break;

			case ALLEGRO_KEY_P:
				Locator::get<AudioManager>()->getSound("space")->play();
				break;

			case ALLEGRO_KEY_F:
				Locator::get<Window>()->toggleFullscreen();
				break;

			case ALLEGRO_KEY_A:
				moving::west = true;
				break;

			case ALLEGRO_KEY_D:
				moving::east = true;
				break;

			case ALLEGRO_KEY_SPACE:
					moving::north = true;
				break;
		}
	break;

	case ALLEGRO_EVENT_KEY_UP:
		switch (event->keyboard.keycode)
		{
		case ALLEGRO_KEY_A:
			moving::west = false;
			break;

		case ALLEGRO_KEY_D:
			moving::east = false;
			break;

		case ALLEGRO_KEY_SPACE:
			moving::north = false;
			break;
		}
		break;
	}

	if (moving::north)
	{
		Locator::get<World>()->m_systemManager.system<MoveSystem>()->jump(5);
	}

	if (moving::west)
	{
		Locator::get<World>()->m_systemManager.system<MoveSystem>()->move(-5);
	}

	if (moving::east)
	{
		Locator::get<World>()->m_systemManager.system<MoveSystem>()->move(5);
	}
}

void Test::update(double dt)
{
	static double counter = 0.0;
	counter += dt;

	constexpr auto time = Time::seconds(3.5);

	if (counter >= time)
	{
		LOG_S(INFO) << "Boop!" << std::endl;
		counter = 0.0f;
	}
}

void Test::render()
{
	Locator::get<World>()->m_systemManager.system<RenderSystem>()->render();
}