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
	AudioManager::AudioManager(const std::string& script)
	{
		int sampleCounter = 0;

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
			m_audioMap.emplace(it.first, it.second);
			++sampleCounter;
		}

		al_reserve_samples(sampleCounter);
	}

	AudioManager::~AudioManager()
	{
		m_audioMap.clear();
	}

	Sound* AudioManager::get(const std::string& id)
	{
		return &(m_audioMap.at(id));
	}
}