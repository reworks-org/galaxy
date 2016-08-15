//
//  Menu.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "Menu.hpp"

#include <SFML/Window/Event.hpp>

#include "re/systems/state/StateIdentifiers.hpp"

using namespace re;

Menu::Menu()
{

}

Menu::~Menu()
{

}

bool Menu::Event(const sf::Event & e)
{
	/*
	if (guiButtonPress)
	{
		RequestStackPop();
		RequestStackPush(StateID::game);
	}
	*/
	return true;
}

bool Menu::Update(double dt)
{
	return true;
}

void Menu::Render()
{

}