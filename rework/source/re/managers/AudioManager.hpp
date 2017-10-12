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
		///
		AudioManager(const std::string& script);

		///
		/// Cleanup fonts.
		///
		~AudioManager() override;

		
		///
		/// Retrieve a sound.
		///
		/// \param id The key of the sound to retrieve.
		///
		/// \return Returns a pointer to the sound.
		///
		Sound* get(const std::string& id);

	private:
		std::unordered_map<std::string, Sound> m_audioMap;
	};
}

#endif
