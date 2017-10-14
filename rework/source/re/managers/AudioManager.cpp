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
		sol::state lua;
		lua.script(Locator::get<VFS>()->openAsString(script));
		sol::table audio = lua.get<sol::table>("audio");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		audio.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& it : m_keyValuePair)
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

		al_reserve_samples(reserveSamples);
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
}