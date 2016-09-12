//
//  Game.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include "Game.hpp"

Game::Game()
	:State()
{
}

Game::~Game()
{
}

bool Game::Event(sf::Event & e)
{
	return true;
}

bool Game::Update(sf::Time dt)
{
	return true;
}

void Game::Render()
{

}