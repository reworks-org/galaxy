//
//  Game.hpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_GAME_HPP_
#define SANDBOX_GAME_HPP_

#include "re/systems/state/State.hpp"

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
	bool Event(const sf::Event& e) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Update state.
	*/
	bool Update(double dt) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Render state.
	*/
	void Render() override;
};

#endif