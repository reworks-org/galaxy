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
#include <re/utility/Serialization.hpp>

#include "gamesystems/MoveSystem.hpp"

#include "Menu.hpp"
#include "Game.hpp"

using namespace re;

std::shared_ptr<State> Menu::m_menuState = std::make_shared<Menu>();

std::shared_ptr<State> Menu::inst()
{
	return m_menuState;
}

Menu::~Menu()
{
}

void Menu::loadResources()
{
	m_window = Locator::get<Window>();
	m_world = Locator::get<World>();
	m_vfs = Locator::get<VFS>();
	m_config = Locator::get<ConfigReader>();

	m_world->registerEntitys("menuEntitys.lua");

	m_world->getEntity("person").get<EventComponent>()->submitOnEvent(Event::MOUSE_PRESSED, []() { RE_LOG_PRINTPRETTY(LogLevel::INFO, "Click!"); });

	m_world->getSystem<MoveSystem>()->submit(m_world);
	m_world->getSystem<RenderSystem>()->submit(m_world);
	m_world->getSystem<EventSystem>()->submit(m_world);
	m_world->getSystem<PhysicsSystem>()->submit(m_world);
	m_world->getSystem<AnimationSystem>()->submit(m_world);

	m_gui.setWindow(*(m_window));

	if (m_doOnce)
	{
		m_theme = std::make_shared<tgui::Theme>("bin/Release/assets/ui/black.txt");
		m_doOnce = false;
	}

	tgui::Button::Ptr button = tgui::loadButtonWithScript(m_theme, "ui/testbutton.lua");
	button->connect("pressed", []() {Locator::get<StateManager>()->changeState(Game::inst()); });
	m_gui.add(button, "testbutton");

	tgui::Label::Ptr time = tgui::loadLabelWithScript(m_theme, "ui/testlabel.lua");
	m_gui.add(time, "testlabel");

	Locator::get<Box2DManager>()->m_collisionFunctions.emplace(std::make_pair("ground", "person"), [](Entity* a, Entity* b) { b->get<PhysicsComponent>()->m_isMovingVertically = false;});
}

void Menu::unloadResources()
{
	m_gui.removeAllWidgets();

	m_world->getSystem<RenderSystem>()->clean();
	m_world->getSystem<EventSystem>()->clean();
	m_world->getSystem<MoveSystem>()->clean();
	m_world->getSystem<PhysicsSystem>()->clean();
	m_world->clean();
}

void Menu::event()
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
		Serialization::save(m_world, "test.sf");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		Serialization::load(m_world, "test.sf");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_world->getEntity("person").get<AnimationComponent>()->play();
		m_world->getSystem<MoveSystem>()->move("person", -5);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_world->getEntity("person").get<AnimationComponent>()->play();
		m_world->getSystem<MoveSystem>()->move("person", 5);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_world->getEntity("person").get<AnimationComponent>()->pause();
		m_world->getSystem<MoveSystem>()->jump("person", 5);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_world->getSystem<EventSystem>()->dispatch(Event::MOUSE_PRESSED);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
		m_world->killEntity("person");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_world->restoreEntity("person");
	}
}

void Menu::update(sf::Time dt)
{
	m_world->update(dt);
	m_world->getSystem<AnimationSystem>()->update(dt);
	m_world->getSystem<PhysicsSystem>()->update(dt);

	m_gui.get<tgui::Label>("testlabel")->setText(Time::getShortTime());
}

void Menu::render()
{
	m_window->clear(sf::Color::Black);

	m_world->getSystem<RenderSystem>()->render(m_window);
	m_gui.draw();

	m_window->display();
}