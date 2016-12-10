//
//  Sound.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SOUND_HPP_
#define RENGINE3_SOUND_HPP_

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "re/scripting/sol2/sol.hpp"
#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class Sound : public sf::Sound
	{
	public:
		/*
		* IMPORTS: table of sound data
		* EXPORTS: none
		* PURPOSE: Set up the class.
		*/
		Sound(sol::table& table);
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up.
		*/
		~Sound();

		/*
		* IMPORTS: name of the file in the vfs.
		* EXPORTS: none
		* PURPOSE: Load a sound from the VFS.
		*/
		void LoadSound(const std::string& name);

	private:
		sf::physfs m_soundStream;
		sf::SoundBuffer m_soundBuffer;
	};
}

#endif