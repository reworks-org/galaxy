//
//  Window.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

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

	void Window::LoadIcon(const std::string& iconName)
	{
		m_iconStream.open(iconName);
		m_windowIcon.loadFromStream(m_iconStream);

		int width = m_windowIcon.getSize().x;
		int height = m_windowIcon.getSize().y;

		setIcon(width, height, m_windowIcon.getPixelsPtr());
	}
}