//
//  Load.hpp
//  REngine3
//
//  Created by reworks on 4/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_LOAD_HPP_
#define SANDBOX_LOAD_HPP_

#include <re/systems/StateManager.hpp>

class Load : public re::State
{
public:
	~Load() override;

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
	void loadResources() override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Unload the states resources.
	*/
	void unloadResources() override;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Process state events.
	*/
	void event() override;

	/*
	* IMPORTS: none
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
	static std::shared_ptr<re::State> m_loadState;

	int m_alpha = 255;
	sf::Time m_timePassed;
};

#endif