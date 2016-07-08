#include "Window.hpp"
//
//  Window.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

namespace re
{
	sf::Window& Window::Get()
	{
		return m_window;
	}
}