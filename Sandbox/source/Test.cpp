//
//  Test.cpp
//  Sandbox
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <allegro5/events.h>
#include <re/core/World.hpp>
#include <re/utils/Time.hpp>
#include <loguru/loguru.hpp>
#include <re/graphics/Window.hpp>
#include <re/systems/RenderSystem.hpp>
#include <re/managers/AudioManager.hpp>
#include <re/services/ServiceLocator.hpp>

#include "Test.hpp"

using namespace re;

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
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		Locator::get<Window>()->close();

	case ALLEGRO_EVENT_KEY_DOWN:
		switch (event->keyboard.keycode)
		{
			case ALLEGRO_KEY_ESCAPE:
				Locator::get<Window>()->close();
				break;

			case ALLEGRO_KEY_SPACE:
				Locator::get<AudioManager>()->getSound("space")->play();
				break;

			case ALLEGRO_KEY_F:
				Locator::get<Window>()->toggleFullscreen(true);
				break;

			case ALLEGRO_KEY_W:
				Locator::get<Window>()->toggleFullscreen(false);
				break;
		}
		break;
	}
}

void Test::update(double dt)
{
	static double counter = 0.0;
	counter += dt;

	constexpr auto time = Time::seconds(5);

	if (counter >= time)
	{
		LOG_S(INFO) << "Boop!" << std::endl;
		counter = 0.0;
	}
}

void Test::render()
{
	Locator::get<World>()->m_systemManager.system<RenderSystem>()->render();
}