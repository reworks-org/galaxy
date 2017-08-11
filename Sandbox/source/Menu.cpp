//
//  Menu.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <iostream>

#include <re/types/Events.hpp>
#include <re/app/World.hpp>
#include <re/entity/Entity.hpp>
#include <re/graphics/Window.hpp>
#include <re/services/Config.hpp>
#include <re/services/VFS.hpp>
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
#include <re/debug/DebugManager.hpp>

#include "gamesystems/MoveSystem.hpp"

#include "Menu.hpp"
#include "Load.hpp"

using namespace re;

std::shared_ptr<State> Menu::m_menuState = std::make_shared<Menu>();

std::shared_ptr<State> Menu::inst()
{
	return m_menuState;
}

Menu::~Menu()
{
}

void Menu::load()
{
	m_window = Locator::get<Window>();
	m_world = Locator::get<World>();
	m_vfs = Locator::get<VFS>();

	m_world->registerEntitys("menuEntitys.lua");

	m_world->getEntity("person").get<EventComponent>()->submitOnEvent(Events::MOUSE_PRESSED, []() { RE_LOG_PRINTPRETTY(LogLevel::INFO, "Click!"); });

	m_gui.setWindow(*(m_window));

	if (m_doneOnce)
	{
		#ifdef _WIN32
			m_theme = std::make_shared<tgui::Theme>("bin/assets/ui/black.txt");
		#else
			m_theme = std::make_shared<tgui::Theme>("Sandbox.app/Contents/Resources/ui/black.txt");
		#endif
		
		m_doneOnce = true;
	}

	tgui::Button::Ptr button = tgui::loadButtonWithScript(m_theme, "ui/testbutton.lua");
	button->connect("pressed", []() {Locator::get<StateManager>()->changeState(Load::inst()); });
	m_gui.add(button, "testbutton");

	tgui::Label::Ptr time = tgui::loadLabelWithScript(m_theme, "ui/testlabel.lua");
	m_gui.add(time, "testlabel");

	Locator::get<Box2DManager>()->m_collisionFunctions.emplace(std::make_pair("ground", "person"), [](const std::string & a, const std::string& b)
	{ 
		Locator::get<World>()->getEntity("person").get<PhysicsComponent>()->m_isMovingVertically = false;
	});

	m_minimap.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
	m_minimap.zoom(1.5f);
}

void Menu::unload()
{
	m_gui.removeAllWidgets();

	Locator::get<Box2DManager>()->clean();
	m_world->getSystem<RenderSystem>()->clean();
	m_world->getSystem<EventSystem>()->clean();
	m_world->getSystem<MoveSystem>()->clean();
	m_world->getSystem<PhysicsSystem>()->clean();
	m_world->getSystem<AnimationSystem>()->clean();

	m_world->clean();

	m_window->setView(m_window->getDefaultView());

}

void Menu::handlePollEvents(sf::Event& event)
{
    m_gui.handleEvent(m_window->m_event);
    
    switch (event.type)
    {
        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:
                    m_world->getSystem<EventSystem>()->dispatch(Events::MOUSE_PRESSED);
                    break;
            }
            break;
    }
}

void Menu::handleEvents(sf::Event& event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        m_window->close();
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        Serialization::save(m_world, "test.sf");
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        Serialization::load(m_world, "test.sf");
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
         m_world->getSystem<MoveSystem>()->move("person", -5);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_world->getSystem<MoveSystem>()->move("person", 5);
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        m_world->getSystem<MoveSystem>()->jump("person", 5);
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
	m_window->setView(m_window->getDefaultView());
	m_world->getSystem<RenderSystem>()->render(m_window);

	m_window->setView(m_minimap);
	m_world->getSystem<RenderSystem>()->render(m_window);

	m_gui.draw();
}