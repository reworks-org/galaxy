//
//  SoundComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/utility/Log.hpp"
#include "re/debug/imgui/imgui-SFML.h"

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

	void SoundComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static std::vector<std::string> soundFiles;
		static int index = 0;
		static bool done = false;
		static std::string originalEntityName = curEntityName;
		static bool isLoop = true;
		static float volume = 0.0f;

		if (originalEntityName != curEntityName)
		{
			originalEntityName = curEntityName;
			done = false;
			index = 0;
		}

		if (!done)
		{
			soundFiles.clear();

			for (auto& it : m_sounds)
			{
				soundFiles.push_back(it.first);
			}

			isLoop = m_sounds[soundFiles[index]].second->getLoop();
			volume = m_sounds[soundFiles[index]].second->getVolume();
			done = true;
		}

		ImGui::SFML::Combo("Sound Selector", &index, soundFiles);

		size_t size = soundFiles.size();
		if ((size_t)index >= size)
		{
			index = (size - 1);
		}

		ImGui::Spacing();
		ImGui::SliderFloat("Volume", &volume, 0, 100, "%.2f");
		m_sounds[soundFiles[index]].second->setVolume(volume);

		ImGui::Spacing();
		ImGui::Checkbox("Is Looping?", &isLoop);
		m_sounds[soundFiles[index]].second->setLoop(isLoop);

		ImGui::Spacing();
		if (ImGui::Button("Play selected sound"))
		{
			m_sounds[soundFiles[index]].second->play();
		}
	}
}
