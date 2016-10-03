//
//  Menu.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <iostream>

#include <SFML/Window/Event.hpp>

#include "re/types/Event.hpp"
#include "re/app/World.hpp"
#include "re/entity/Entity.hpp"
#include "re/graphics/Window.hpp"
#include "re/services/Config.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/systems/EventSystem.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/EventComponent.hpp"
#include "re/systems/state/StateIdentifiers.hpp"
#include "re/entity/EntityManager.hpp"

#include "re/graphics/ui/UILabel.hpp"
#include "re/graphics/ui/UISystem.hpp"

#include "Menu.hpp"

using namespace re;

bool dragging = false;

Menu::Menu()
	:State()
{
	m_window = Locator::Get<Window>();
	m_world = Locator::Get<World>();
	m_vfs = Locator::Get<VFS>();
	m_config = Locator::Get<ConfigReader>();
	m_manager = Locator::Get<EntityManager>();

	m_world->Register("menuEntitys.lua");
	
	m_world->Get<UISystem>()->AddPanel("menupanel", std::make_shared<UIPanel>("menupanel.lua"));
	m_world->Get<UISystem>()->GetPanel("menupanel")->Add("label", std::make_shared<UILabel>("menulabel.lua"));

	m_world->Get<UISystem>()->GetPanel("menupanel")->SetComponentOffsets();

	m_manager->At("person")->Get<AnimatedSpriteComponent>()->Play();
	m_manager->At("person")->Get<EventComponent>()->SubmitOnEvent(Event::MOUSE_PRESSED, [] {std::cout << "Event processed!" << std::endl; });

	m_world->Get<RenderSystem>()->AutoSubmit();
	m_world->Get<EventSystem>()->AutoSubmit();
}

Menu::~Menu()
{
}

bool Menu::Event(sf::Event & e)
{
	m_world->Get<UISystem>()->Event();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_world->Get<EventSystem>()->Dispatch(Event::MOUSE_PRESSED);
		
		dragging = true;
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		dragging = false;
	}

	return true;
}

bool Menu::Update(sf::Time dt)
{
	if (dragging)
	{
		m_manager->At("person")->Get<AnimatedSpriteComponent>()->setPosition(sf::Mouse::getPosition(*(m_window)).x, sf::Mouse::getPosition(*(m_window)).y);
	}

	m_world->Get<RenderSystem>()->Update(dt);

	m_world->Get<UISystem>()->Update();

	m_world->Update(dt);
	return true;
}

void Menu::Render()
{
	m_window->clear(sf::Color::White);

	m_world->Get<RenderSystem>()->Render(m_window);
	m_world->Get<UISystem>()->Render(m_window);

	m_window->display();
}