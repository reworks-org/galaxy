//
//  MusicComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/utility/Log.hpp"

#include "MusicComponent.hpp"

namespace re
{
	MusicComponent::MusicComponent()
	{
	}

	MusicComponent::~MusicComponent()
	{
		for (auto& it : m_music)
		{
			it.second.second->stop();
		}

		m_music.clear();
	}

	void MusicComponent::init(sol::table& table)
	{
		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		table.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

        RE_REVERSE_ASSERT(m_keyValuePair.empty(), "Tried to load music with no data", "MusicComponent::init", "MusicComponent.cpp", 39);
        
		for (auto& kvp : m_keyValuePair)
		{
			m_music.emplace(kvp.first, std::make_pair(std::make_unique<sf::physfs>(), std::make_unique<sf::Music>()));
			m_music[kvp.first].first->open(kvp.second.get<std::string>("file"));
            
			RE_ASSERT(m_music[kvp.first].second->openFromStream(*(m_music[kvp.first].first)), kvp.first + " did not load", "MusicComponent::init", "MusicComponent.cpp", 46);
            
			m_music[kvp.first].second->setVolume(kvp.second.get<float>("volume"));
			m_music[kvp.first].second->setLoop(kvp.second.get<bool>("looping"));
		}
	}
}
