//
//  UISystem.hpp
//  REngine3
//
//  Created by reworks on 29/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_UISYSTEM_HPP_
#define RENGINE3_UISYSTEM_HPP_

#include <string>

#include "re/types/System.hpp"
#include "re/graphics/ui/UIPanel.hpp"

namespace re
{
	class Window;

	class UISystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up system.
		*/
		~UISystem() override;

		/*
		* IMPORTS: id of panel and a poiner to a new panel.
		* EXPORTS: none
		* PURPOSE: Add a new panel to the screen. All UI is draw relative to the panel.
		*/
		void AddPanel(const std::string& id, std::shared_ptr<UIPanel> panel);

		/*
		* IMPORTS: id of panel.
		* EXPORTS: pointer to panel.
		* PURPOSE: Get a panel from the map.
		*/
		std::shared_ptr<UIPanel> GetPanel(const std::string& id);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Handle events for all ui panels and components.
		*/
		void Event();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update ALL ui panels and components.
		*/
		void Update();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Render ALL ui panels and components.
		*/
		void Render(re::Window* window);

	private:
		std::unordered_map<std::string, std::shared_ptr<UIPanel>> m_panels;
	};
}

#endif