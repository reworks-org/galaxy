//
//  SoundComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/utility/Log.hpp"

#include "SoundComponent.hpp"

namespace re
{
	SoundComponent::SoundComponent()
	{
        m_sounds.clear();
	}

	SoundComponent::~SoundComponent()
	{
		for (auto& it : m_sounds)
		{
			it.second.second->stop();
		}

		m_sounds.clear();
	}

	void SoundComponent::init(sol::table& table)
	{
		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		table.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

        RE_REVERSE_ASSERT(m_keyValuePair.empty(), "Tried to load a sound component with no data", "SoundComponent::init", "SoundComponent.cpp", 39);
        
		for (auto& kvp : m_keyValuePair)
		{
			m_sounds.emplace(kvp.first, std::make_pair(std::make_pair(std::make_unique<sf::SoundBuffer>(), std::make_unique<sf::physfs>()), std::make_unique<sf::Sound>()));
			m_sounds[kvp.first].first.second->open(kvp.second.get<std::string>("file"));

			RE_ASSERT(m_sounds[kvp.first].first.first->loadFromStream(*(m_sounds[kvp.first].first.second)), kvp.first + " did not load", "SoundComponent::init", "SoundComponent.cpp", 46);

			m_sounds[kvp.first].second->setBuffer(*(m_sounds[kvp.first].first.first));
			m_sounds[kvp.first].second->setVolume(kvp.second.get<float>("volume"));
			m_sounds[kvp.first].second->setLoop(kvp.second.get<bool>("looping"));
		}
	}

	void SoundComponent::debugFunction(sol::table& table)
	{
	}
}
