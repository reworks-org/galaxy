///
///  StateManager.cpp
///  rework
///
///  Created by reworks on 5/10/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "re/types/BaseState.hpp"

#include "StateManager.hpp"

namespace re
{
	StateManager::~StateManager()
	{
		m_currentState.reset();
	}

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

	void StateManager::changeState(std::shared_ptr<BaseState> s)
	{
		m_currentState->unload();
		m_currentState = s;
		m_currentState->load();
	}

	void StateManager::reloadState(std::shared_ptr<BaseState> s)
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

	void StateManager::setState(std::shared_ptr<BaseState> s)
	{
		m_currentState = s;
	}
}