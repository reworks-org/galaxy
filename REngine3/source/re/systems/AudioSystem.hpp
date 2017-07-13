//
//  AudioSystem.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_AUDIOSYSTEM_HPP_
#define RENGINE3_AUDIOSYSTEM_HPP_

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>

#include "re/types/System.hpp"

namespace re
{
	class AudioSystem : public System
	{
	public:
		/*
		* IMPORTS: default volume for system.
		* EXPORTS: none
		* PURPOSE: Construct system
		*/
		AudioSystem(int defaultEffectVolume, int defaultMusicVolume);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the system.
		*/
		~AudioSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Sets up audio entitys.
		*/
		void prepareAudio();

		/*
		* IMPORTS: id of entity to add and its component list.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void addEntity(Entity* e) override;

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void removeEntity(const std::string& name) override;

		/*
		* IMPORTS: accessor in format: <entityName>.<soundeffectname>
		* EXPORTS: Pointer to sound.
		* PURPOSE: Get a sound in the system.
		*/
		sf::Sound& getSound(const std::string& accessor);

		/*
		* IMPORTS: accessor in format: <entityName>.<musicname>
		* EXPORTS: Pointer to music.
		* PURPOSE: Get music in the system.
		*/
		sf::Music& getMusic(const std::string& accessor);

		/*
		* IMPORTS: volume (0-100).
		* EXPORTS: none
		* PURPOSE: Change the volume of all music.
		*/
		void setMusicVolume(int newVolume);

		/*
		* IMPORTS: volume (0-100).
		* EXPORTS: none
		* PURPOSE: Change the volume of all sounds.
		*/
		void setEffectVolume(int newVolume);

		/*
		* IMPORTS: none
		* EXPORTS: volume (0-100)
		* PURPOSE: retrieve default volume
		*/
		int getEffectVolume() const;

		/*
		* IMPORTS: none
		* EXPORTS: volume (0-100)
		* PURPOSE: retrieve default volume
		*/
		int getMusicVolume() const;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Stops music.
		*/
		void stop();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void clean() override;

	private:
		int m_effectVolume;
		int m_musicVolume;
	};
}

#endif
