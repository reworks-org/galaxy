//
//  StateManager.cpp
//  REngine3
//
//  Created by reworks on 5/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "StateManager.hpp"

namespace re
{
	State::~State()
	{
	}

	void StateManager::changeState(std::shared_ptr<State> s)
	{
	    m_currentState->unloadResources();
		m_currentState = s;
		m_currentState->loadResources();
	}

	void StateManager::setState(std::shared_ptr<State> s)
	{
		m_currentState = s;
	}

	void StateManager::event(sf::Event& event)
	{
		m_currentState->event(event);
	}

	void StateManager::update(sf::Time dt)
	{
		m_currentState->update(dt);
	}

	void StateManager::render()
	{
		m_currentState->render();
	}

	void StateManager::loadResources()
	{
		m_currentState->loadResources();
	}

	void StateManager::unloadResources()
	{
		m_currentState->unloadResources();
	}
}
