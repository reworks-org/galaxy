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
	Window::Window(int width, int height)
	:m_running(true)
	{
		m_display = al_create_display(width, height);
	}

	Window::~Window()
	{
		al_destroy_display(m_window);
	}

	void Window::close()
	{
		m_running = false;
	}

	ALLEGRO_DISPLAY* Window::getDisplay()
	{
		return m_display;
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
