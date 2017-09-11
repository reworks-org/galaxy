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
	}

	ParallaxComponent::~ParallaxComponent()
	{
	}

	void ParallaxComponent::init(sol::table& table)
	{
		m_texture.loadFromFile(Locator::get<VFS>()->retrieve(table.get<std::string>("texture")));
		m_sprite.setTexture(m_texture);

		sol::table parallaxList = table.get<sol::table>("parallaxList");

		std::map<std::string, sol::table> m_keyValuePairParallax;
		parallaxList.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePairParallax.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		RE_REVERSE_ASSERT(m_keyValuePairParallax.empty(), "Attempted to use an empty parllax list", "ParallaxComponent::init", "ParallaxComponent.cpp", 39);

		for (auto& it : m_keyValuePairParallax)
		{
			m_parallaxMaps.emplace(it.second.get<sf::Uint32>("layer"), sf::Rect<int>(it.second.get<int>("x"), it.second.get<int>("y"), it.second.get<int>("w"), it.second.get<int>("h")));
		}
	}

	sf::Drawable* ParallaxComponent::getDrawable()
	{
		return m_sprite;
	}

	void ParallaxComponent::debugFunction(sol::table& table, const std::string& curEntityName)
	{
		ImGui::Spacing();

		ImGui::Text("Please modify from script, then reload state.");
	}

	const std::unordered_map<sf::Uint32, sf::Rect<int>>& ParallaxComponent::getParallaxRects() const
	{
		return m_parallaxMaps;
	}
}