//
//  Music.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_MUSIC_HPP_
#define RENGINE3_MUSIC_HPP_

#include <SFML/Audio/Music.hpp>

#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class Music : public sf::Music
	{
	public:
		/*
		* IMPORTS: Music script
		* EXPORTS: none
		* PURPOSE: Set up the class.
		*/
		Music(const std::string& script);
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up.
		*/
		~Music();

		/*
		* IMPORTS: name of the file in the vfs.
		* EXPORTS: none
		* PURPOSE: Load a music from the VFS.
		*/
		void LoadMusic(const std::string& name);

	private:
		sf::physfs m_musicStream;
	};
}

#endif