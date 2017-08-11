//
//  Menu.hpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_MENU_HPP_
#define SANDBOX_MENU_HPP_

#include <re/services/StateManager.hpp>
#include <re/scripting/TGUISol2Bridge.hpp>

class Menu : public re::State
{
public:
	~Menu() override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Instance of state.
	*/
	static std::shared_ptr<re::State> inst();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Load the states resources.
	*/
	void load() override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Unload the states resources.
	*/
	void unload() override; 

    /*
     * IMPORTS: none
     * EXPORTS: none
     * PURPOSE: Process state events.
     */
    void handlePollEvents(sf::Event& event) override;
    
    /*
     * IMPORTS: none
     * EXPORTS: none
     * PURPOSE: Process state events.
     */
    void handleEvents(sf::Event& event) override;

	/*
	* IMPORTS: delta time.
	* EXPORTS: none
	* PURPOSE: Update state.
	*/
	void update(sf::Time dt) override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Render state.
	*/
	void render() override;

private:
	static std::shared_ptr<re::State> m_menuState;
	tgui::Gui m_gui;
	tgui::Theme::Ptr m_theme;
	sf::View m_minimap;
};

#endif
