//
//  Menu.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <iostream>

#include <re/types/Event.hpp>
#include <re/app/World.hpp>
#include <re/entity/Entity.hpp>
#include <re/graphics/Window.hpp>
#include <re/services/Config.hpp>
#include <re/services/vfs/VFS.hpp>
#include <re/systems/MoveSystem.hpp>
#include <re/systems/EventSystem.hpp>
#include <re/entity/EntityManager.hpp>
#include <re/systems/RenderSystem.hpp>
#include <re/services/ServiceLocator.hpp>
#include <re/component/EventComponent.hpp>
#include <re/component/PositionComponent.hpp>

#include "Menu.hpp"
#include "Game.hpp"

using namespace re;

std::shared_ptr<State> Menu::m_menuState = std::make_shared<Menu>();

std::shared_ptr<State> Menu::Inst()
{
	return m_menuState;
}

Menu::~Menu()
{
}

void Menu::LoadResources()
{
	m_window = Locator::Get<Window>();
	m_world = Locator::Get<World>();
	m_vfs = Locator::Get<VFS>();
	m_config = Locator::Get<ConfigReader>();
	m_manager = Locator::Get<EntityManager>();

	m_world->Register("menuEntitys.lua");

	m_manager->At("person")->Get<AnimatedSpriteComponent>()->Play();
	m_manager->At("person")->Get<EventComponent>()->SubmitOnEvent(Event::MOUSE_PRESSED, [] { RE_LOG(LogLevel::INFO, "Event processed!"); });

	m_world->Get<MoveSystem>()->AutoSubmit();
	m_world->Get<RenderSystem>()->AutoSubmit();
	m_world->Get<EventSystem>()->AutoSubmit();

	gui.setWindow(*(m_window));

	if (m_doOnce)
	{
		theme = std::make_shared<tgui::Theme>("bin/Release/assets/ui/black.txt");
		m_doOnce = false;
	}

	tgui::Button::Ptr button = theme->load("Button");
	button->setSize(100, 100);
	button->setPosition(500, 10);
	button->setText("Go to Game");
	button->connect("pressed", [&](){Locator::Get<StateManager>()->ChangeState(Game::Inst());});

	gui.add(button, "test");
}

void Menu::UnloadResources()
{
	gui.removeAllWidgets();

	m_world->Get<RenderSystem>()->Clean();
	m_world->Get<EventSystem>()->Clean();
	m_world->Get<MoveSystem>()->Clean();
	m_manager->Clean();
	m_world->Clean();
}

void Menu::Event(sf::Event& e)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_world->Get<EventSystem>()->Dispatch(Event::MOUSE_PRESSED);
		
		m_dragging = true;
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_dragging = false;
	}

	while (m_window->pollEvent(e))
	{
		gui.handleEvent(e);
	}
}

void Menu::Update(sf::Time dt)
{
	if (m_dragging)
	{
		m_manager->At("person")->Get<PositionComponent>()->Update(sf::Mouse::getPosition(*(m_window)).x, sf::Mouse::getPosition(*(m_window)).y);
	}

	m_world->Get<MoveSystem>()->Update(dt);
	m_world->Get<RenderSystem>()->Update(dt);

	m_world->Update(dt);
}

void Menu::Render()
{
	m_window->clear(sf::Color::Black);

	m_world->Get<RenderSystem>()->Render(m_window);
	gui.draw();

	m_window->display();
}