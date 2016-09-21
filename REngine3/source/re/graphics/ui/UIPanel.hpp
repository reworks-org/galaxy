//
//  UIPanel.hpp
//  REngine3
//
//  Created by reworks on 29/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_UIPANEL_HPP_
#define RENGINE3_UIPANEL_HPP_

#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "re/graphics/ui/UIComponent.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class Window;

	class UIPanel
	{
	public:
		/*
		* IMPORTS: script - The lua script defining the uipanel.
		* EXPORTS: none
		* PURPOSE: Construct the panel.
		*/
		UIPanel(const std::string& script);
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up Panel.
		*/
		~UIPanel();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Add a new uicomponent to the system.
		*/
		void Add(const std::string& id, std::shared_ptr<UIComponent> ui);

		/*
		* IMPORTS: type and id of uicomponent.
		* EXPORTS: UIComponent
		* PURPOSE: Get a UIComponent.
		*/
		template<typename T>
		std::shared_ptr<T> Get(const std::string& id);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Call this function to set all components offset positions.
		*/
		void SetComponentOffsets();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Is the panel visible or invisible.
		*/
		void SetVisibility(bool isVisible);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process events with ui.
		*/
		void Event();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update UI components in panel.
		*/
		void Update();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Draw everything to the screen.
		*/
		void Render(re::Window* window);
	
		/*
		* IMPORTS: id - the id of the component.
		* EXPORTS: none
		* PURPOSE: Removes a component from the panel, and deletes it.
		*/
		void DeleteComponent(const std::string& id);

		/*
		* IMPORTS: id - the id of the component.
		* EXPORTS: none
		* PURPOSE: Restores a deleted component to the map.
		*/
		void RestoreComponent(const std::string& id);

	private:
		std::unordered_map<std::string, std::shared_ptr<UIComponent>> m_uicomponents;
		std::unordered_map<std::string, std::shared_ptr<UIComponent>> m_deletedComponents;

		sf::Sprite m_sprite;
		sf::IntRect m_spriteArea;

		bool m_isVisible = true;

		sf::physfs m_textureStream;
		sf::Texture m_texture;

		int m_xpos;
		int m_ypos;
	};

	template<typename T>
	std::shared_ptr<T> UIPanel::Get(const std::string& id)
	{
		return std::dynamic_pointer_cast<T>(m_UIComponents[id]);
	}
}

#endif