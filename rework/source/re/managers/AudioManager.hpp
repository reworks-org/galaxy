//
//  AudioManager.hpp
//  rework
//
//  Created by reworks on 12/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_AUDIOMANAGER_HPP_
#define REWORK_AUDIOMANAGER_HPP_

#include <unordered_map>

#include "re/audio/Sound.hpp"
#include "re/audio/Music.hpp"
#include "re/types/Service.hpp"

namespace re
{
	class AudioManager : public Service
	{
	public:
		///
		/// \brief Construct AudioManager and add sounds or music.
		///
		/// \param script Script file containing sound/music data to parse.
		/// \param reserveSamples Allegro samples to reserve. Basically the number of sounds playing simultaneously.
		///
		AudioManager(const std::string& script, int reserveSamples);

		///
		/// Cleanup fonts.
		///
		~AudioManager() override;
		
		///
		/// Retrieve a sound.
		///
		/// \param name The name of the sound to retrieve.
		///
		/// \return Returns a pointer to the sound.
		///
		Sound* getSound(const std::string& name);

		///
		/// Retrieve a music file.
		///
		/// \param name The name of the music to retrieve.
		///
		/// \return Returns a pointer to the music.
		///
		Music* getMusic(const std::string& name);

	private:
		std::unordered_map<std::string, Sound> m_soundMap;
		std::unordered_map<std::string, Music> m_musicMap;
	};
}

#endif
