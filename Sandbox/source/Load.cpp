//
//  Load.cpp
//  REngine3
//
//  Created by reworks on 4/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Window/Event.hpp>

#include "re/app/World.hpp"
#include "re/services/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/systems/MoveSystem.hpp"
#include "re/entity/EntityManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/systems/RenderSystem.hpp"

#include "Load.hpp"
#include "Menu.hpp"

using namespace re;

std::shared_ptr<State> Load::m_loadState = std::make_shared<Load>();

std::shared_ptr<State> Load::Inst()
{
	return m_loadState;
}

Load::~Load()
{
}

void Load::LoadResources()
{
	m_window = Locator::Get<Window>();
	m_world = Locator::Get<World>();
	m_vfs = Locator::Get<VFS>();
	m_config = Locator::Get<ConfigReader>();
	m_manager = Locator::Get<EntityManager>();

	m_world->Register("loadEntitys.lua");

	m_manager->At("loadScreen")->Get<SpriteComponent>()->setColor(sf::Color(255, 255, 255, m_alpha));

	m_world->Get<MoveSystem>()->AutoSubmit();
	m_world->Get<RenderSystem>()->AutoSubmit();
}

void Load::UnloadResources()
{
	m_world->Get<RenderSystem>()->Clean();
	m_world->Get<MoveSystem>()->Clean();
	m_manager->Clean();
	m_world->Clean();
}

void Load::Event(sf::Event& e, StateSystem* stateManager)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_window->close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		stateManager->ChangeState(Menu::Inst());
	}
}

void Load::Update(sf::Time dt, StateSystem* stateManager)
{
	m_world->Get<MoveSystem>()->Update(dt);
	m_world->Get<RenderSystem>()->Update(dt);

	m_timePassed += dt;

	if (m_timePassed > sf::seconds(1))
	{
		m_alpha -= 3;

		if (m_alpha < 0)
		{
			m_alpha = 0;
		}

		m_manager->At("loadScreen")->Get<SpriteComponent>()->setColor(sf::Color(255, 255, 255, m_alpha));

		if (m_alpha <= 0)
		{
			stateManager->ChangeState(Menu::Inst());
		}
	}

	m_world->Update(dt);
}

void Load::Render(StateSystem* stateManager)
{
	m_window->clear(sf::Color::Black);

	m_world->Get<RenderSystem>()->Render(m_window);

	m_window->display();
}