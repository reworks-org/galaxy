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
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Process state events.
	*/
	void Event() override;

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
	static std::shared_ptr<re::State> m_loadState;

	int m_alpha = 255;
	sf::Time m_timePassed;
};

#endif