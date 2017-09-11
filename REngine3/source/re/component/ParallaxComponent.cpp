//
//  ParallaxComponent.cpp
//  REngine3
//
//  Created by reworks on 29/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <map>

#include "re/services/VFS.hpp"
#include "re/debug/imgui/imgui-sfml.h"
#include "re/services/ServiceLocator.hpp"

#include "ParallaxComponent.hpp"

namespace re
{
	ParallaxComponent::ParallaxComponent()
	{
		m_parallaxMaps.clear();
	}

	ParallaxComponent::~ParallaxComponent()
	{
		m_parallaxMaps.clear();
	}

	void ParallaxComponent::init(sol::table& table)
	{
		m_texture.loadFromFile(Locator::get<VFS>()->retrieve(table.get<std::string>("texture")));
		sol::table parallaxList = table.get<sol::table>("parallaxList");

		std::map<std::string, sol::table> m_keyValuePairParallax;
		parallaxList.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePairParallax.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		RE_REVERSE_ASSERT(m_keyValuePairParallax.empty(), "Attempted to use an empty parllax list", "ParallaxComponent::init", "ParallaxComponent.cpp", 39);

		for (auto& it : m_keyValuePairParallax)
		{
			sf::Uint32 layer = it.second.get<sf::Uint32>("layer");
			m_parallaxMaps.emplace(layer, sf::Sprite());
			m_parallaxMaps[layer].setTexture(m_texture);
			m_parallaxMaps[layer].setTextureRect(sf::Rect<int>(it.second.get<int>("x"), it.second.get<int>("y"), it.second.get<int>("w"), it.second.get<int>("h")));
			m_parallaxMaps[layer].setPosition(it.second.get<float>("xpos"), it.second.get<float>("ypos"));
		}
	}

	void ParallaxComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		ImGui::Spacing();

		ImGui::Text("Please modify from script, then reload state.");
	}

	std::unordered_map<sf::Uint32, sf::Sprite>& ParallaxComponent::getParallaxMap()
	{
		return m_parallaxMaps;
	}
}