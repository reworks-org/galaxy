//
//  Load.hpp
//  REngine3
//
//  Created by reworks on 4/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_LOAD_HPP_
#define SANDBOX_LOAD_HPP_

#include "re/systems/StateSystem.hpp"

class Load : public re::State
{
public:
	~Load() override;

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

private:
	static std::shared_ptr<re::State> m_loadState;

	int m_alpha = 255;
	sf::Time m_timePassed;
};

#endif