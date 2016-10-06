//
//  Game.hpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_GAME_HPP_
#define SANDBOX_GAME_HPP_

#include "re/systems/StateSystem.hpp"

class Game : public re::State
{
public:
	Game();
	~Game() override;

	/*
	* IMPORTS: sf::Event object. 
	* EXPORTS: none
	* PURPOSE: Process state events.
	*/
	void Event(sf::Event& e, re::StateSystem* stateManager) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Update state.
	*/
	void Update(sf::Time dt, re::StateSystem* stateManager) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Render state.
	*/
	void Render(re::StateSystem* stateManager) override;
};

#endif