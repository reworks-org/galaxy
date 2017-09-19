//
//  SoundComponent.cpp
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
			m_sounds.emplace(kvp.first, std::make_pair(sf::SoundBuffer(), std::make_unique<sf::Sound>()));

			RE_ASSERT(m_sounds[kvp.first].first.loadFromFile(Locator::get<VFS>()->retrieve(kvp.second.get<std::string>("file"))), kvp.first + " did not load", "SoundComponent::init", "SoundComponent.cpp", 49);

			m_sounds[kvp.first].second->setBuffer(m_sounds[kvp.first].first);
			m_sounds[kvp.first].second->setLoop(kvp.second.get<bool>("looping"));

			sol::object obj = kvp.second["spacial"];
			if (obj != sol::nil)
			{
				sol::table spacial = table.get<sol::table>("spacial");
				m_sounds[kvp.first].second->setPosition(spacial.get<float>("x"), spacial.get<float>("y"), spacial.get<float>("z"));
				m_sounds[kvp.first].second->setRelativeToListener(spacial.get<bool>("relativeToListener"));
				m_sounds[kvp.first].second->setAttenuation(spacial.get<float>("attenuation"));
				m_sounds[kvp.first].second->setMinDistance(spacial.get<float>("minDistance"));
			}
		}
	}

	void SoundComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		static std::vector<std::string> soundFiles;
		static int index = 0;
		static bool done = false;
		static std::string originalEntityName = curEntityName;
		static bool isLoop = true;
		static bool rel = true;
		static float pos[] = { 0.0f, 0.0f, 0.0f };
		static float att = 1.0f;
		static float dis = 1.0f;

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
			pos[0] = m_sounds[soundFiles[index]].second->getPosition().x;
			pos[1] = m_sounds[soundFiles[index]].second->getPosition().y;
			pos[2] = m_sounds[soundFiles[index]].second->getPosition().z;
			rel = m_sounds[soundFiles[index]].second->isRelativeToListener();
			att = m_sounds[soundFiles[index]].second->getAttenuation();
			dis = m_sounds[soundFiles[index]].second->getMinDistance();

			done = true;
		}

		ImGui::SFML::Combo("Sound Selector", &index, soundFiles);

		size_t size = soundFiles.size();
		if ((size_t)index >= size)
		{
			index = (size - 1);
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Is Looping?", &isLoop))
		{
			m_sounds[soundFiles[index]].second->setLoop(isLoop);
		}

		ImGui::Spacing();
		if (ImGui::Button("Play selected sound"))
		{
			m_sounds[soundFiles[index]].second->play();
		}

		ImGui::Spacing();
		ImGui::Text("Spacialization Modifiers: ");

		ImGui::Spacing();
		if (ImGui::InputFloat3("pos modifier", pos))
		{
			m_sounds[soundFiles[index]].second->setPosition(pos[0], pos[1], pos[2]);
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Relative To Listener", &rel))
		{
			m_sounds[soundFiles[index]].second->setRelativeToListener(rel);
		}

		ImGui::Spacing();
		if (ImGui::SliderFloat("Attenuation", &att, 0.0f, 100.0f))
		{
			m_sounds[soundFiles[index]].second->setAttenuation(att);
		}

		ImGui::Spacing();
		if (ImGui::SliderFloat("Min Distance", &dis, 0.1f, 100.0f))
		{
			m_sounds[soundFiles[index]].second->setMinDistance(dis);
		}
	}
}
