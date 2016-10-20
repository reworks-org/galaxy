//
//  Menu.hpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_MENU_HPP_
#define SANDBOX_MENU_HPP_

#include <TGUI/TGUI.hpp>

#include <re/systems/StateManager.hpp>

class Menu : public re::State
{
public:
	~Menu() override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Instance of state.
	*/
	static std::shared_ptr<re::State> Inst();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Load the states resources.
	*/
	void LoadResources() override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Unload the states resources.
	*/
	void UnloadResources() override; 

	/*
	* IMPORTS: sf::Event object.
	* EXPORTS: none
	* PURPOSE: Process state events.
	*/
	void Event(sf::Event& e) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Update state.
	*/
	void Update(sf::Time dt) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Render state.
	*/
	void Render() override;

private:
	static std::shared_ptr<re::State> m_menuState;
	tgui::Gui gui;
	tgui::Theme::Ptr theme;

	bool m_dragging = false;
	bool m_doOnce = true;
};

#endif