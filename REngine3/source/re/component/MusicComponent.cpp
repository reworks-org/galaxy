//
//  MusicComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "MusicComponent.hpp"

namespace re
{
	MusicComponent::MusicComponent()
	{
	}

	MusicComponent::~MusicComponent()
	{
	}

	void MusicComponent::Init(sol::table& table)
	{
		// Get key-value pairs from table
		std::map<std::string, std::string> m_keyValuePair;
		table.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<std::string>() });
		});

		for (auto& kvp : m_keyValuePair)
		{
			m_music.emplace(kvp.first, std::make_shared<Music>(kvp.second));
		}
	}
}