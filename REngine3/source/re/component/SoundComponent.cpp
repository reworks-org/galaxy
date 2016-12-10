//
//  SoundComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "SoundComponent.hpp"

namespace re
{
	SoundComponent::SoundComponent()
	{
	}

	SoundComponent::~SoundComponent()
	{
	}

	void SoundComponent::Init(sol::table& table)
	{
		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		table.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& kvp : m_keyValuePair)
		{
			m_sounds.emplace(kvp.first, std::make_shared<Sound>(kvp.second));
		}
	}
}