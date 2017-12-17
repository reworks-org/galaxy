//
//  AudioManager.cpp
//  rework
//
//  Created by reworks on 22/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <map>

#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "AudioManager.hpp"

namespace re
{
	AudioManager::AudioManager(const std::string& script, int reserveSamples)
	{
		al_restore_default_mixer();
		al_reserve_samples(reserveSamples);

		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(script));
		sol::table audio = lua.get<sol::table>("audio");

		// Get key-value pairs from table
		std::map<std::string, sol::table> kvp;
		audio.for_each([&](std::pair<sol::object, sol::object> pair) {
			kvp.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& it : kvp)
		{
			if (it.second.get<std::string>("type") == "sound")
			{
				m_soundMap.emplace(it.first, it.second);
			}
			else if (it.second.get<std::string>("type") == "music")
			{
				m_musicMap.emplace(it.first, it.second);
			}
		}
	}

	AudioManager::~AudioManager()
	{
		m_soundMap.clear();
		m_musicMap.clear();
	}

	Sound* AudioManager::getSound(const std::string& name)
	{
		return &(m_soundMap.at(name));
	}

	Music* AudioManager::getMusic(const std::string& name)
	{
		return &(m_musicMap.at(name));
	}

	void AudioManager::changeMusicVolume(float volume)
	{
		if (volume > 1.0) { volume = 1.0f; }
		if (volume < 0.0) { volume = 0.0f; }

		for (auto& it : m_musicMap)
		{
			it.second.setVolume(volume);
		}
	}

	void AudioManager::changeSoundVolume(float volume)
	{
		if (volume > 1.0) { volume = 1.0f; }
		if (volume < 0.0) { volume = 0.0f; }

		for (auto& it : m_soundMap)
		{
			it.second.setVolume(volume);
		}
	}
}