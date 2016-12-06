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
#include <re/systems/EventSystem.hpp>
#include <re/systems/RenderSystem.hpp>
#include <re/utility/Serialization.hpp>
#include <re/systems/PhysicsSystem.hpp>
#include <re/services/ServiceLocator.hpp>
#include <re/component/EventComponent.hpp>
#include <re/component/TransformComponent.hpp>
#include <re/component/AnimationComponent.hpp>
#include <re/systems/AnimationSystem.hpp>
#include <re/physics/Box2DManager.hpp>
#include <re/component/PhysicsComponent.hpp>
#include <re/utility/Time.hpp>

#include "gamesystems/MoveSystem.hpp"

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

	m_world->Register("menuEntitys.lua");

	m_world->Get("person").Get<EventComponent>()->SubmitOnEvent(Event::MOUSE_PRESSED, []() { RE_LOG(LogLevel::INFO, "Click!"); });

	m_world->Get<MoveSystem>()->AutoSubmit(m_world);
	m_world->Get<RenderSystem>()->AutoSubmit(m_world);
	m_world->Get<EventSystem>()->AutoSubmit(m_world);
	m_world->Get<PhysicsSystem>()->AutoSubmit(m_world);
	m_world->Get<AnimationSystem>()->AutoSubmit(m_world);

	m_gui.setWindow(*(m_window));

	if (m_doOnce)
	{
		m_theme = std::make_shared<tgui::Theme>("bin/Release/assets/ui/black.txt");
		m_doOnce = false;
	}

	tgui::Button::Ptr button = tgui::loadButtonWithScript(m_theme, "ui/testbutton.lua");
	button->connect("pressed", []() {Locator::Get<StateManager>()->ChangeState(Game::Inst()); });
	m_gui.add(button, "testbutton");

	tgui::Label::Ptr time = tgui::loadLabelWithScript(m_theme, "ui/testlabel.lua");
	m_gui.add(time, "testlabel");

	Locator::Get<Box2DManager>()->m_collisionFunctions.emplace(std::make_pair("ground", "person"), [](Entity* a, Entity* b) { b->Get<PhysicsComponent>()->m_isMovingVertically = false;});
}

void Menu::UnloadResources()
{
	m_gui.removeAllWidgets();

	m_world->Get<RenderSystem>()->Clean();
	m_world->Get<EventSystem>()->Clean();
	m_world->Get<MoveSystem>()->Clean();
	m_world->Get<PhysicsSystem>()->Clean();
	m_world->Clean();
}

void Menu::Event()
{
	// TODO: fix movement and jumping

	while (m_window->pollEvent(m_window->m_event))
	{
		switch (m_window->m_event.type)
		{
		case sf::Event::Closed:
			m_window->close();
			break;
		}

		m_gui.handleEvent(m_window->m_event);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		Serialization::Save(m_world, "test.sf");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		Serialization::Load(m_world, "test.sf");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_world->Get("person").Get<AnimationComponent>()->Play();
		m_world->Get<MoveSystem>()->Move("person", -5);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_world->Get("person").Get<AnimationComponent>()->Play();
		m_world->Get<MoveSystem>()->Move("person", 5);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_world->Get("person").Get<AnimationComponent>()->Play();
		m_world->Get<MoveSystem>()->Jump("person", 5);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_world->Get<EventSystem>()->Dispatch(Event::MOUSE_PRESSED);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
		m_world->KillEntity("person");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_world->ReviveEntity("person");
	}
}

void Menu::Update(sf::Time dt)
{
	m_world->Update(dt);
	m_world->Get<AnimationSystem>()->Update(dt);
	m_world->Get<PhysicsSystem>()->Update(dt);

	m_gui.get<tgui::Label>("testlabel")->setText(Time::GetShortTimeAndDate());
}

void Menu::Render()
{
	m_window->clear(sf::Color::Black);

	m_world->Get<RenderSystem>()->Render(m_window);
	m_gui.draw();

	m_window->display();
}