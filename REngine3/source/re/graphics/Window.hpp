//
//  Window.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WINDOW_HPP_
#define RENGINE3_WINDOW_HPP_

#include <SFML/Window.hpp>

namespace re
{
	class Window
	{
	public:
		sf::Window& Get();

	private:
		int m_screenWidth;
		int m_screenHeight;
		std::string m_title;

		sf::Window m_window;
	};
}

#endif