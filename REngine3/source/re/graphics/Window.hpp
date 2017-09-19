//
//  Window.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_WINDOW_HPP_
#define RENGINE3_WINDOW_HPP_

#include <allegro5/display.h>

namespace re
{
	class Window
	{
	public:
		///
		/// \brief Sets up window.
		/// 
		/// Sets up all the data for the allegro window such as rendering.
		///
		Window();

		///
		/// Cleans up the window.
		///
		~Window();

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

		void close();

		ALLEGRO_DISPLAY* getDisplay();

	private:
		bool m_running;
		ALLEGRO_DISPLAY* m_display;
	};
}

#endif
