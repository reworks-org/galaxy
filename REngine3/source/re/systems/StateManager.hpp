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
#include <unordered_map>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "re/types/Service.hpp"

namespace re
{
	class VFS;
	class World;
	class Window;
	class ConfigReader;
	class StateManager;

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
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process the current states events.
		*/
	    virtual void Event() = 0;

		/*
		* IMPORTS: delta time
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
		Window* m_window;
		ConfigReader* m_config;
		World* m_world;
		VFS* m_vfs;
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
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process the current states events.
		*/
	    void Event();

		/*
		* IMPORTS: delta time
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