//
//  DebugManager.hpp
//  REngine3
//
//  Created by reworks on 20/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_DEBUGMANAGER_HPP_
#define RENGINE3_DEBUGMANAGER_HPP_

#include "re/app/World.hpp"
#include "re/scripting/sol2/sol.hpp"

namespace re
{
	class State;

    class DebugManager : public Service
    {
    public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default constructor.
		*/
		DebugManager();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up IMGUI.
		*/
		~DebugManager() override;

		/*
		* IMPORTS: renderwindow or target
		* EXPORTS: none
		* PURPOSE: To set up IMGUI.
		*/
		void init(sf::RenderTarget& target, bool debugEnabled);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process IMGUI events.
		*/
		void event(const sf::Event& event);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update IMGUI.
		*/
		void update(sf::RenderWindow& window, sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Draw IMGUI.
		*/
        void render(sf::RenderWindow& window);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: call between update and render. Calls the functions that make up the main debug menu.
		*/
		void useMenu();

		/*
		* IMPORTS: state pointer to reload to, and a function to cleanup anything that normally isnt cleaned up at that time.
		* EXPORTS: none
		* PURPOSE: Set a state to reload too.
		* NOTE: func() is called first when reloading.
		*/
		void specifyReloadState(std::shared_ptr<State> s, std::function<void(void)> func);

	private:
		std::shared_ptr<State> m_reloadState;
		std::function<void(void)> m_reloadFunc;
		bool m_enabled;
        bool m_init;
        sol::state m_lua;
		re::World* m_world;
    };
}

#endif
