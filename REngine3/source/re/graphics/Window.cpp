//
//  Window.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Window.hpp"

namespace re
{
	Window::Window()
	:sf::RenderWindow()
	{
	}

	Window::Window(sf::VideoMode mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings)
	:sf::RenderWindow(mode, title, style, settings)
	{
	}

	Window::Window(sf::WindowHandle handle, const sf::ContextSettings& settings)
	:sf::RenderWindow(handle, settings)
	{
	}

	Window::~Window()
	{
	}

	void Window::make(int screenwidth, int screenheight, const std::string & title, sf::Uint32 style)
	{
		m_screenWidth = screenwidth;
		m_screenHeight = screenheight;
		m_title = title;
		m_style = style;

		create(sf::VideoMode(screenwidth, screenheight), title, style);
	}

	void Window::goFullscreen(bool value)
	{
		if (value == true)
		{
			create(sf::VideoMode(m_screenWidth, m_screenHeight), m_title, m_style | sf::Style::Fullscreen);
		}
		else
		{
			create(sf::VideoMode(m_screenWidth, m_screenHeight), m_title, m_style);
		}
	}

	void Window::loadIcon(const std::string& iconName)
	{
		m_windowIcon.loadFromFile(Locator::get<VFS>()->retrieve(iconName));

		int width = m_windowIcon.getSize().x;
		int height = m_windowIcon.getSize().y;

		setIcon(width, height, m_windowIcon.getPixelsPtr());
	}
}
