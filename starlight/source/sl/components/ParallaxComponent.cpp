///
/// ParallaxComponent.cpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// Copyright (c) 2018+ reworks.
/// Refer to LICENSE.txt for more details.
///

#include "sol2/sol.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "ParallaxComponent.hpp"

namespace sl
{
	ParallaxComponent::ParallaxComponent(const sol::table& table)
	{
		table.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			sol::table internalTable = pair.second.as<sol::table>();
			m_parallaxSprites.emplace_back(internalTable.get<float>("velX"), internalTable.get<float>("velY"), internalTable.get<float>("speed"), entt::HashedString(internalTable.get<const char*>("id")));
		});

		std::sort(m_parallaxSprites.begin(), m_parallaxSprites.end(), [](const ParallaxSprite& a, const ParallaxSprite& b)
		{
			return a.m_layer > b.m_layer;
		});
	}
}