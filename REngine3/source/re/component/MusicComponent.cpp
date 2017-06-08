//
//  MusicComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/utility/Log.hpp"
#include "re/debug/imgui/imgui-SFML.h"

#include "MusicComponent.hpp"

namespace re
{
	MusicComponent::MusicComponent()
	{
        m_music.clear();
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

	void MusicComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static std::vector<std::string> musicFiles;
		static int index = 0;
		static bool done = false;
		static std::string originalEntityName = curEntityName;
		static bool isLoop = true;
		static sf::Time duration = sf::Time::Zero;
		static float volume = 0.0f;

		if (originalEntityName != curEntityName)
		{
			originalEntityName = curEntityName;
			done = false;
			index = 0;
		}

		if (!done)
		{
			musicFiles.clear();

			for (auto& it : m_music)
			{
				musicFiles.push_back(it.first);
			}

			isLoop = m_music[musicFiles[index]].second->getLoop();
			duration = m_music[musicFiles[index]].second->getDuration();
			volume = m_music[musicFiles[index]].second->getVolume();
			done = true;
		}

		ImGui::SFML::Combo("Music Selector", &index, musicFiles);

		size_t size = musicFiles.size();
		if ((size_t)index >= size)
		{
			index = (size - 1);
		}

		ImGui::Spacing();
		std::string text = "Duration: " + std::to_string(duration.asSeconds()) + " seconds";
		ImGui::Text(text.c_str());

		ImGui::Spacing();
		if (ImGui::SliderFloat("Volume", &volume, 0, 100, "%.2f"))
		{
			m_music[musicFiles[index]].second->setVolume(volume);
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Is Looping?", &isLoop))
		{
			m_music[musicFiles[index]].second->setLoop(isLoop);
		}

		ImGui::Spacing();
		if (ImGui::Button("Play selected music"))
		{
			m_music[musicFiles[index]].second->play();
		}

		ImGui::Spacing();
		if (ImGui::Button("Stop selected music"))
		{
			m_music[musicFiles[index]].second->stop();
		}
	}
}
