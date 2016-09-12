//
//  StateSystem.cpp
//  REngine3
//
//  Created by reworks on 10/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/Window/Event.hpp>

#include "StateSystem.hpp"

namespace re
{
	StateSystem::~StateSystem()
	{
		m_stack.clear();
		m_pendingList.clear();
		m_factory.clear();
	}

	void StateSystem::Event(sf::Event& e)
	{
		for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
		{
			if (!(*itr)->Event(e))
				break;
		}

		ApplyPendingChanges();
	}

	void StateSystem::Update(sf::Time dt)
	{
		for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
		{
			if (!(*itr)->Update(dt))
				break;
		}

		ApplyPendingChanges();
	}

	void StateSystem::Render()
	{
		for (std::unique_ptr<State>& state : m_stack)
		{
			state->Render();
		}
	}

	void StateSystem::PushState(int stateID)
	{
		m_pendingList.push_back(PendingChange(Push, stateID));
	}

	void StateSystem::PopState()
	{
		m_pendingList.push_back(PendingChange(Pop));
	}

	void StateSystem::ClearStates()
	{
		m_pendingList.push_back(PendingChange(Clear));
	}

	bool StateSystem::IsEmpty() const
	{
		return m_stack.empty();
	}

	std::unique_ptr<State> StateSystem::CreateState(int stateID)
	{
		auto found = m_factory.find(stateID);
		if (found != m_factory.end())
		{
			return found->second();
		}
		else
		{
			return nullptr;
		}
	}

	void StateSystem::ApplyPendingChanges()
	{
		for (PendingChange change : m_pendingList)
		{
			switch (change.action)
			{
			case Push:
				m_stack.push_back(CreateState(change.stateID));
				break;

			case Pop:
				m_stack.pop_back();
				break;

			case Clear:
				m_stack.clear();
				break;
			}
		}

		m_pendingList.clear();
	}

	StateSystem::PendingChange::PendingChange(Action action, int stateID)
	: action(action)
	, stateID(stateID)
	{
	}
}