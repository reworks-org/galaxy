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

namespace re
{
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
		* IMPORTS: renderwindow or target, or a custom font.
		* EXPORTS: none
		* PURPOSE: To set up IMGUI.
		*/
		void init(sf::RenderTarget& target, sf::Texture* fontTexture = NULL);

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
		* PURPOSE: Clean up IMGUI.
		*/
		void cleanup();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: disables the debug manager
		*/
		void disable();

		/*
		* IMPORTS: none
		* EXPORTS: boolean
		* PURPOSE: Checks if the debug manager is disabled.
		*/
		bool isEnabled();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: enables the debug manager.
		*/
		void enable();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: call between update and render. Calls the functions that make up the main debug menu.
		*/
		void useMenu();

	private:
		bool m_enabled;
        bool m_init = false;
		EntityList* m_alive;
		EntityList* m_dead;
    };
}

#endif
