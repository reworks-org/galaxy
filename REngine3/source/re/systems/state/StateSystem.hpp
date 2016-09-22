//
//  StateSystem.hpp
//  REngine3
//
//  Created by reworks on 10/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_STATESYSTEM_HPP_
#define RENGINE3_STATESYSTEM_HPP_

// Thanks to SFML Game Development Book.
// You can find it here: https://www.packtpub.com/game-development/sfml-game-development

#include <map>
#include <memory>
#include <utility>
#include <functional>

#include <SFML/System/Time.hpp>

#include "re/types/System.hpp"
#include "re/systems/state/State.hpp"
#include "re/systems/state/StateIdentifiers.hpp"

namespace sf
{
	class Event;
}

namespace re
{
	class StateSystem : public System
	{
	public:
		/*
		* Enum of actions the stack can perform.
		*/
		enum Action
		{
			Push,
			Pop,
			Clear
		};

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the system.
		*/
		~StateSystem();

		/*
		* IMPORTS: stateID - the id of the state.
		* EXPORTS: none
		* PURPOSE: Register a state with the state stack.
		*/
		template<typename T>
		void RegisterState(int stateID);

		/*
		* IMPORTS: e - SFML event object.
		* EXPORTS: none
		* PURPOSE: Process the current states events.
		*/
		void Event(sf::Event& e);
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Updates the current state.
		*/
		void Update(sf::Time dt);
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Render the current state.
		*/
		void Render();

		/*
		* IMPORTS: stateID - Id of the state to push.
		* EXPORTS: none
		* PURPOSE: Push a new state onto the stack.
		*/
		void PushState(int stateID);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Pop a state from the stack.
		*/
		void PopState();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clear all the states from a stack.
		*/
		void ClearStates();

		/*
		* IMPORTS: none
		* EXPORTS: boolean - true if empty.
		* PURPOSE: Checks if the stack is empty.
		*/
		bool IsEmpty() const;

	private:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Create state based on id.
		*/
		std::unique_ptr<State> CreateState(int stateID);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Apply changes to the stack.
		*/
		void ApplyPendingChanges();

	private:
		struct PendingChange
		{
			explicit PendingChange(Action action, int stateID = StateID::none);
			Action action;
			int stateID;
		};

	private:
		std::vector<std::unique_ptr<State>> m_stack;
		std::vector<PendingChange> m_pendingList;

		std::map<int, std::function<std::unique_ptr<State>()>> m_factory;
	};

	template<typename T>
	void StateSystem::RegisterState(int stateID)
	{
		m_factory[stateID] = [this]()
		{
			return std::unique_ptr<State>(new T());
		};
	}
}

#endif