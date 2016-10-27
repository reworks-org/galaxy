//
//  Game.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Graphics/View.hpp>

#include <re/app/World.hpp>
#include <re/mapping/Level.hpp>
#include <re/graphics/Window.hpp>
#include <re/services/Config.hpp>
#include <re/services/vfs/VFS.hpp>
#include <re/services/ServiceLocator.hpp>

#include "levels/TestLevel.hpp"

#include "Game.hpp"
#include "Menu.hpp"

using namespace re;

std::shared_ptr<State> Game::m_gameState = std::make_shared<Game>();

sf::View minimap;

std::shared_ptr<State> Game::Inst()
{
	return m_gameState;
}

Game::~Game()
{
}

void Game::LoadResources()
{
	m_window = Locator::Get<Window>();
	m_world = Locator::Get<World>();
	m_vfs = Locator::Get<VFS>();
	m_config = Locator::Get<ConfigReader>();

	minimap.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
	m_levels.emplace("test", std::make_shared<TestLevel>("testLevel.lua", m_window));
	m_currentLevel = m_levels["test"];
}

void Game::UnloadResources()
{
	m_window->setView(m_window->getDefaultView());
}

void Game::Event()
{
	while (m_window->pollEvent(m_window->m_event))
	{
		switch (m_window->m_event.type)
		{
		case sf::Event::Closed:
			m_window->close();
			break;
		}

		m_currentLevel->Event(m_window->m_event);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		Locator::Get<StateManager>()->ChangeState(Menu::Inst());
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_currentLevel->Move(0, -3);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_currentLevel->Move(0, 3);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_currentLevel->Move(-3, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_currentLevel->Move(3, 0);
	}
}

void Game::Update(sf::Time dt)
{
	m_currentLevel->Update(dt);
}

void Game::Render()
{
	m_window->clear(sf::Color::Black);

	m_window->draw(*(m_currentLevel));

	m_window->display();
}