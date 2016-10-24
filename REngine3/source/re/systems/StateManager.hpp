//
//  StateManager.hpp
//  REngine3
//
//  Created by reworks on 5/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_STATEMANAGER_HPP_
#define RENGINE3_STATEMANAGER_HPP_

#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "re/types/Service.hpp"

namespace re
{
	class StateManager;
	class VFS;
	class World;
	class Window;
	class ConfigReader;

	class State
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Virtual destructor.
		*/
		virtual ~State();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Load the states resources.
		*/
	    virtual void LoadResources() = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Unload the states resources.
		*/
	    virtual void UnloadResources() = 0;
	    
		/*
		* IMPORTS: e - SFML event object.
		* EXPORTS: none
		* PURPOSE: Process the current states events.
		*/
	    virtual void Event(sf::Event& e) = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Updates the current state.
		*/
	    virtual void Update(sf::Time dt) = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Render the current state.
		*/
	    virtual void Render() = 0;

	protected:
		// Services
		re::Window* m_window;
		re::ConfigReader* m_config;
		re::World* m_world;
		re::VFS* m_vfs;
	};

	class StateManager : public Service
	{
	public:	    
		/*
		* IMPORTS: pointer to new state
		* EXPORTS: none
		* PURPOSE: Change the current state. This unloads and reloads resources.
		*/
	    void ChangeState(std::shared_ptr<State> s);

		/*
		* IMPORTS: pointer to new state
		* EXPORTS: none
		* PURPOSE: This changes the state but WITHOUT CHANGING RESOURCES!
		*/
	    void SetState(std::shared_ptr<State> s);
	    
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
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Load the states resources.
		*/
	    void LoadResources();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Unload the states resources.
		*/
	    void UnloadResources();
			    
	private:
	    std::shared_ptr<State> m_currentState;
	};
}

#endif