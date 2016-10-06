//
//  StateSystem.cpp
//  REngine3
//
//  Created by reworks on 5/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "StateSystem.hpp"

namespace re
{
	State::~State()
	{
	}

	void StateSystem::ChangeState(std::shared_ptr<State> s)
	{
	    m_currentState->UnloadResources();
		m_currentState = s;
		m_currentState->LoadResources();
	}

	void StateSystem::SetState(std::shared_ptr<State> s)
	{
		m_currentState = s;
	}

	void StateSystem::Event(sf::Event& e)
	{
		m_currentState->Event(e, this);
	}

	void StateSystem::Update(sf::Time dt)
	{
		m_currentState->Update(dt, this);
	}

	void StateSystem::Render()
	{
		m_currentState->Render(this);
	}

	void StateSystem::LoadResources()
	{
		m_currentState->LoadResources();
	}

	void StateSystem::UnloadResources()
	{
		m_currentState->UnloadResources();
	}
}