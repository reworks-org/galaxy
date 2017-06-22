//
//  Load.cpp
//  REngine3
//
//  Created by reworks on 4/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <re/app/World.hpp>
#include <re/services/Config.hpp>
#include <re/graphics/Window.hpp>
#include <re/services/VFS.hpp>
#include <re/systems/RenderSystem.hpp>
#include <re/services/ServiceLocator.hpp>
#include <re/component/SpriteComponent.hpp>

#include "gamesystems/MoveSystem.hpp"

#include "Load.hpp"
#include "Menu.hpp"

using namespace re;

std::shared_ptr<State> Load::m_loadState = std::make_shared<Load>();

std::shared_ptr<State> Load::inst()
{
	return m_loadState;
}

Load::~Load()
{
}

void Load::loadResources()
{
	m_window = Locator::get<Window>();
	m_world = Locator::get<World>();
	m_vfs = Locator::get<VFS>();

	m_world->registerEntitys("loadEntitys.lua");
	m_world->getEntity("loadScreen").get<SpriteComponent>()->setColor(sf::Color(255, 255, 255, m_alpha));

	m_world->getSystem<RenderSystem>()->submit(m_world);
}

void Load::unloadResources()
{
	m_world->getSystem<RenderSystem>()->clean();
	m_world->clean();
}

void Load::handlePollEvents(sf::Event& event)
{
}

void Load::handleEvents(sf::Event& event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        m_window->close();
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        Locator::get<StateManager>()->changeState(Menu::inst());
    }
}

void Load::update(sf::Time dt)
{
	m_world->update(dt);
	
	m_timePassed += dt;

	if (m_timePassed > sf::seconds(1))
	{
		m_alpha -= 3;

		if (m_alpha < 0)
		{
			m_alpha = 0;
		}

		m_world->getEntity("loadScreen").get<SpriteComponent>()->setColor(sf::Color(255, 255, 255, m_alpha));

		if (m_alpha <= 0)
		{
			Locator::get<StateManager>()->changeState(Menu::inst());
		}
	}
}

void Load::render()
{
	m_world->getSystem<RenderSystem>()->render(m_window);
}
