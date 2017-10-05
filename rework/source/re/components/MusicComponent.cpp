//
//  MusicComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/utility/Log.hpp"
#include "re/services/VFS.hpp"
#include "re/debug/imgui/imgui-SFML.h"
#include "re/services/ServiceLocator.hpp"

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
			it.second->stop();
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
			m_music.emplace(kvp.first, std::make_unique<sf::Music>());
            
			RE_ASSERT(m_music[kvp.first]->openFromFile(Locator::get<VFS>()->retrieve(kvp.second.get<std::string>("file"))), kvp.first + " did not load", "MusicComponent::init", "MusicComponent.cpp", 49);
            
			m_music[kvp.first]->setLoop(kvp.second.get<bool>("looping"));

			sol::object obj = kvp.second["spacial"];
			if (obj != sol::nil)
			{
				sol::table spacial = table.get<sol::table>("spacial");
				m_music[kvp.first]->setPosition(spacial.get<float>("x"), spacial.get<float>("y"), spacial.get<float>("z"));
				m_music[kvp.first]->setRelativeToListener(spacial.get<bool>("relativeToListener"));
				m_music[kvp.first]->setAttenuation(spacial.get<float>("attenuation"));
				m_music[kvp.first]->setMinDistance(spacial.get<float>("minDistance"));
			}
		}
	}

	void MusicComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static std::vector<std::string> musicFiles;
		static int index = 0;
		static bool done = false;
		static std::string originalEntityName = curEntityName;
		static bool isLoop = true;
		static bool rel = true;
		static float pos[] = { 0.0f, 0.0f, 0.0f };
		static float att = 1.0f;
		static float dis = 1.0f;
		static sf::Time duration = sf::Time::Zero;

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

			isLoop = m_music[musicFiles[index]]->getLoop();
			duration = m_music[musicFiles[index]]->getDuration();
			pos[0] = m_music[musicFiles[index]]->getPosition().x;
			pos[1] = m_music[musicFiles[index]]->getPosition().y;
			pos[2] = m_music[musicFiles[index]]->getPosition().z;
			rel = m_music[musicFiles[index]]->isRelativeToListener();
			att = m_music[musicFiles[index]]->getAttenuation();
			dis = m_music[musicFiles[index]]->getMinDistance();

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
		ImGui::Text("%s", text.c_str());

		ImGui::Spacing();
		if (ImGui::Checkbox("Is Looping?", &isLoop))
		{
			m_music[musicFiles[index]]->setLoop(isLoop);
		}

		ImGui::Spacing();
		if (ImGui::Button("Play selected music"))
		{
			m_music[musicFiles[index]]->play();
		}

		ImGui::Spacing();
		if (ImGui::Button("Stop selected music"))
		{
			m_music[musicFiles[index]]->stop();
		}

		ImGui::Spacing();
		ImGui::Text("Spacialization Modifiers: ");

		ImGui::Spacing();
		if (ImGui::InputFloat3("pos modifier", pos))
		{
			m_music[musicFiles[index]]->setPosition(pos[0], pos[1], pos[2]);
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Relative To Listener", &rel))
		{
			m_music[musicFiles[index]]->setRelativeToListener(rel);
		}

		ImGui::Spacing();
		if (ImGui::SliderFloat("Attenuation", &att, 0.0f, 100.0f))
		{
			m_music[musicFiles[index]]->setAttenuation(att);
		}

		ImGui::Spacing();
		if (ImGui::SliderFloat("Min Distance", &dis, 0.1f, 100.0f))
		{
			m_music[musicFiles[index]]->setMinDistance(dis);
		}
	}
}
