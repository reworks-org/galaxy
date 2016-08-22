//
//  State.hpp
//  REngine3
//
//  Created by reworks on 25/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_STATE_HPP_
#define RENGINE3_STATE_HPP_

// Thanks to SFML Game Development Book.
// You can find it here: https://www.packtpub.com/game-development/sfml-game-development

#include <memory>

namespace sf
{
	class Event;
}

namespace re
{
	class State
	{
	public:
		State() {}
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Virtual destructor to clean up classes.
		*/
		virtual ~State();

		/*
		* IMPORTS: sf::Event object.
		* EXPORTS: none
		* PURPOSE: Process state events.
		*/
		virtual bool Event(const sf::Event& e) = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update state.
		*/
		virtual bool Update(double dt) = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Render state.
		*/
		virtual void Render() = 0;

	protected:
		/*
		* IMPORTS: stateID - the id of the state.
		* EXPORTS: none
		* PURPOSE: Request a push to the stack.
		*/
		void RequestStackPush(int stateID);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Request a pop to the stack.
		*/
		void RequestStackPop();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Request to clear the stack.
		*/
		void RequestStateClear();

	private:

	};
}

#endif