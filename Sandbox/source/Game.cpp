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
#include <re/entity/EntityManager.hpp>
#include <re/services/ServiceLocator.hpp>

#include "Game.hpp"
#include "Menu.hpp"

using namespace re;

std::shared_ptr<State> Game::m_gameState = std::make_shared<Game>();

sf::Vector2f view_center = sf::Vector2f(1280, 720) / 2.f;
sf::View view(view_center, sf::Vector2f(1280, 720));

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
	m_manager = Locator::Get<EntityManager>();

	map = new tmx::TileMap("bin/Release/assets/desert.tmx");
	map->ShowObjects();
	m_window->setView(view);

	minimap.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
}

void Game::UnloadResources()
{
	m_window->setView(m_window->getDefaultView());

	delete map;
}

void Game::Event(sf::Event& e)
{
	if (e.type == sf::Event::Closed)
	{
		m_window->close();
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
		view.move(0, -3);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		view.move(0, 3);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		view.move(-3, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		view.move(3, 0);
	}
}

void Game::Update(sf::Time dt)
{
	m_window->setView(view);
}

void Game::Render()
{
	m_window->clear(sf::Color::Black);

	m_window->draw(*(map));

	m_window->display();
}