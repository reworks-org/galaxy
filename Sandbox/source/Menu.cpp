//
//  Menu.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "Menu.hpp"

#include <SFML/Window/Event.hpp>

#include "re/entity/World.hpp"
#include "re/entity/Entity.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/systems/state/StateIdentifiers.hpp"

using namespace re;

Menu::Menu()
	:State()
{
	Locator::Get<World>()->AddEntity("menu", std::make_shared<Entity>("menu.lua"));

	Locator::Get<World>()->GetSystem<RenderSystem>()->Submit();
}

Menu::~Menu()
{

}

bool Menu::Event(sf::Event & e)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || e.type == sf::Event::Closed)
	{
		Locator::Get<World>()->m_window.close();
	}

	return true;
}

bool Menu::Update(double dt)
{
	Locator::Get<World>()->GetSystem<RenderSystem>()->Update();

	return true;
}

void Menu::Render()
{
	Locator::Get<World>()->GetSystem<RenderSystem>()->Render();
}