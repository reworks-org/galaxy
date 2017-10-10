//
//  StateManager.cpp
//  rework
//
//  Created by reworks on 5/10/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "StateManager.hpp"

namespace re
{
	void StateManager::load()
	{
		m_currentState->load();
	}

	void StateManager::unload()
	{
		m_currentState->unload();
	}

    void StateManager::event(ALLEGRO_EVENT* event)
    {
        m_currentState->event(event);
    }
    
	void StateManager::update(double dt)
	{
		m_currentState->update(dt);
	}

	void StateManager::render()
	{
		m_currentState->render();
	}

	void StateManager::changeState(std::shared_ptr<State> s)
	{
		m_currentState->unload();
		m_currentState = s;
		m_currentState->load();
	}

	void StateManager::reloadState(std::shared_ptr<State> s)
	{
		m_currentState->unload();

		if (s != nullptr)
		{
			m_currentState = s;
			m_currentState->load();
		}
		else
		{
			m_currentState->load();
		}
	}

	void StateManager::setState(std::shared_ptr<State> s)
	{
		m_currentState = s;
	}
}