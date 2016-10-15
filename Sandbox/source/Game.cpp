//
//  Game.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Window/Event.hpp>

#include "re/types/Event.hpp"
#include "re/app/World.hpp"
#include "re/entity/Entity.hpp"
#include "re/graphics/Window.hpp"
#include "re/services/Config.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/systems/MoveSystem.hpp"
#include "re/graphics/ui/UILabel.hpp"
#include "re/systems/EventSystem.hpp"
#include "re/graphics/ui/UISystem.hpp"
#include "re/entity/EntityManager.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/EventComponent.hpp"
#include "re/component/PositionComponent.hpp"

#include "Game.hpp"

using namespace re;

std::shared_ptr<State> Game::m_gameState = std::make_shared<Game>();

sf::Vector2f view_center = sf::Vector2f(1280, 720) / 2.f;
sf::View view(view_center, sf::Vector2f(1280, 720));

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
}

void Game::UnloadResources()
{
	delete map;
}

void Game::Event(sf::Event& e, StateSystem* stateManager)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	map->
}

void Game::Update(sf::Time dt, StateSystem* stateManager)
{
	
}

void Game::Render(StateSystem* stateManager)
{
	m_window->clear(sf::Color::Black);

	m_window->draw(*(map));

	m_window->display();
}