//
//  Menu.hpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_MENU_HPP_
#define SANDBOX_MENU_HPP_

#include "re/systems/state/State.hpp"

class Menu : public re::State
{
public:
	Menu();
	~Menu() override;

	/*
	* IMPORTS: sf::Event object.
	* EXPORTS: none
	* PURPOSE: Process state events.
	*/
	bool Event(sf::Event& e) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Update state.
	*/
	bool Update(sf::Time dt) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Render state.
	*/
	void Render() override;
};

#endif