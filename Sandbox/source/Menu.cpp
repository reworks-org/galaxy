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
#include "re/graphics/Window.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/systems/state/StateIdentifiers.hpp"
#include "re/services/Config.hpp"
#include "re/services/vfs/VFS.hpp"

#include "re/graphics/ui/UISystem.hpp"
#include "re/graphics/ui/UILabel.hpp"

using namespace re;

Menu::Menu()
	:State()
{
	m_window = Locator::Retrieve<Window>();
	m_world = Locator::Retrieve<World>();
	m_vfs = Locator::Retrieve<VFS>();
	m_config = Locator::Retrieve<ConfigReader>();

	m_world->AddEntity("menu", std::make_shared<Entity>("menu.lua"));
	m_world->AddEntity("person", std::make_shared<Entity>("person.lua"));

	m_world->GetSystem<RenderSystem>()->Submit(m_world);

	m_world->GetSystem<UISystem>()->AddPanel("menupanel", std::make_shared<UIPanel>("menupanel.lua"));
	m_world->GetSystem<UISystem>()->GetPanel("menupanel")->Add("label", std::make_shared<UILabel>("menulabel.lua"));

	m_world->GetSystem<UISystem>()->GetPanel("menupanel")->SetComponentOffsets();

	m_world->GetEntity("person")->Get<AnimatedSpriteComponent>()->Play();
}

Menu::~Menu()
{
}

bool Menu::Event(sf::Event & e)
{
	m_world->GetSystem<UISystem>()->Event();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || e.type == sf::Event::Closed)
	{
		m_window->close();
	}

	return true;
}

bool Menu::Update(sf::Time dt)
{
	m_world->GetSystem<RenderSystem>()->Update(dt);

	m_world->GetSystem<UISystem>()->Update();

	return true;
}

void Menu::Render()
{
	m_window->clear(sf::Color::White);

	m_world->GetSystem<RenderSystem>()->Render(m_window);
	m_world->GetSystem<UISystem>()->Render(m_window);

	m_window->display();
}