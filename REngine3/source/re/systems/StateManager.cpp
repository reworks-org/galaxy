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

	void StateManager::ChangeState(std::shared_ptr<State> s)
	{
	    m_currentState->UnloadResources();
		m_currentState = s;
		m_currentState->LoadResources();
	}

	void StateManager::SetState(std::shared_ptr<State> s)
	{
		m_currentState = s;
	}

	void StateManager::Event()
	{
		m_currentState->Event();
	}

	void StateManager::Update(sf::Time dt)
	{
		m_currentState->Update(dt);
	}

	void StateManager::Render()
	{
		m_currentState->Render();
	}

	void StateManager::LoadResources()
	{
		m_currentState->LoadResources();
	}

	void StateManager::UnloadResources()
	{
		m_currentState->UnloadResources();
	}
}