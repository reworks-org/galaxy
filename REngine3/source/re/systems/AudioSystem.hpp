//
//  AudioSystem.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_AUDIOSYSTEM_HPP_
#define RENGINE3_AUDIOSYSTEM_HPP_

#include "re/types/System.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"

namespace re
{
	class AudioSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the system.
		*/
		~AudioSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void AutoSubmit(World* world) override;

		/*
		* IMPORTS: id of entity to add and its component list.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void AddEntity(Entity* e) override;

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void RemoveEntity(const std::string& name) override;

		/*
		* IMPORTS: accessor in format: <entityName>.<soundeffectname>
		* EXPORTS: Pointer to sound.
		* PURPOSE: Get a sound in the system.
		*/
		sf::Sound* GetSound(const std::string& accessor);

		/*
		* IMPORTS: accessor in format: <entityName>.<musicname>
		* EXPORTS: Pointer to music.
		* PURPOSE: Get music in the system.
		*/
		sf::Music* GetMusic(const std::string& accessor);

		/*
		* IMPORTS: volume (0-100).
		* EXPORTS: none
		* PURPOSE: Change the volume of all music.
		*/
		void SetGlobalMusicVolume(float volume);

		/*
		* IMPORTS: volume (0-100).
		* EXPORTS: none
		* PURPOSE: Change the volume of all sounds.
		*/
		void SetGlobalSoundVolume(float volume);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void Clean() override;
	};
}

#endif