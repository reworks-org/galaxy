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

#include "re/types/Service.hpp"

namespace sf
{
	class Event;
}

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
	    virtual void loadResources() = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Unload the states resources.
		*/
	    virtual void unloadResources() = 0;
	    
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process the current states events.
		*/
	    virtual void event(sf::Event& event) = 0;

		/*
		* IMPORTS: delta time
		* EXPORTS: none
		* PURPOSE: Updates the current state.
		*/
	    virtual void update(sf::Time dt) = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Render the current state.
		*/
	    virtual void render() = 0;

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
	    void changeState(std::shared_ptr<State> s);

		/*
		* IMPORTS: pointer to new state
		* EXPORTS: none
		* PURPOSE: This changes the state but WITHOUT CHANGING RESOURCES!
		*/
	    void setState(std::shared_ptr<State> s);
	    
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process the current states events.
		*/
	    void event(sf::Event& event);

		/*
		* IMPORTS: delta time
		* EXPORTS: none
		* PURPOSE: Updates the current state.
		*/
	    void update(sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Render the current state.
		*/
	    void render();
	    
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Load the states resources.
		*/
	    void loadResources();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Unload the states resources.
		*/
	    void unloadResources();
			    
	private:
	    std::shared_ptr<State> m_currentState;
	};
}

#endif
