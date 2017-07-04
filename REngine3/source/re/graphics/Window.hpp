//
//  Window.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WINDOW_HPP_
#define RENGINE3_WINDOW_HPP_

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "re/types/Service.hpp"

namespace re
{
	class Window : public Service, public sf::RenderWindow
	{
	public:
		/*
		* See sf::RenderWindow Docs!
		*/
		Window();

		/*
		* See sf::RenderWindow Docs!
		*/
		Window(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

		/*
		* See sf::RenderWindow Docs!
		*/
		Window(sf::WindowHandle handle, const sf::ContextSettings& settings = sf::ContextSettings());

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up classes.
		*/
		~Window() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: This is an alternative to create() in renderwindow. This still calls that function, but also preserves the values passed to the function.
		*/
		void make(int screenwidth, int screenheight, const std::string& title, sf::Uint32 style);

		/*
		* IMPORTS: boolean - true is fullscreen.
		* EXPORTS: none
		* PURPOSE: Make the window fullscreen or not.
		*/
		void goFullscreen(bool value);

		/*
		* IMPORTS: name of the icon in the vfs
		* EXPORTS: none
		* PURPOSE: Loads the windows icon, make sure you call after adding VFS to the Locator class.
		*/
		void loadIcon(const std::string& iconName);

	private:
		sf::Image m_windowIcon;
		std::string m_title;
		int m_screenWidth;
		int m_screenHeight;
		sf::Uint32 m_style;

	public:
		sf::Event m_event;
	};
}

#endif
