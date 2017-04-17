//
//  Game.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Graphics/View.hpp>

#include <re/app/World.hpp>
#include <re/graphics/Window.hpp>
#include <re/services/Config.hpp>
#include <re/services/vfs/VFS.hpp>
#include <re/services/ServiceLocator.hpp>

#include "Game.hpp"
#include "Menu.hpp"

using namespace re;

std::shared_ptr<State> Game::m_gameState = std::make_shared<Game>();

int g_speed = 1;

std::shared_ptr<State> Game::inst()
{
	return m_gameState;
}

Game::~Game()
{
}

void Game::loadResources()
{
	m_window = Locator::get<Window>();
	m_world = Locator::get<World>();
	m_vfs = Locator::get<VFS>();
	m_config = Locator::get<ConfigReader>();

	m_map.load("Sandbox.app/Contents/Resources/example.tmx");
}

void Game::unloadResources()
{
	m_window->setView(m_window->getDefaultView());
}

void Game::event()
{
	while (m_window->pollEvent(m_window->m_event))
	{
		switch (m_window->m_event.type)
		{
		case sf::Event::Closed:
			m_window->close();
			break;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		g_speed = 2;
	}
	else
	{
		g_speed = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		Locator::get<StateManager>()->changeState(Menu::inst());
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_map.move(0, 3*g_speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_map.move(0, -3 * g_speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_map.move(3 * g_speed, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_map.move(-3*g_speed, 0);
	}
}

void Game::update(sf::Time dt)
{
	m_map.update(dt);
}

void Game::render()
{
	m_window->clear(sf::Color::Black);

	m_window->draw(m_map);

	m_window->display();
}
