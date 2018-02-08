///
/// SpriteComponent.cpp
/// starlight
///
/// Created by reworks on 12/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "sol2/sol.hpp"

#include "SpriteComponent.hpp"

namespace sl
{
	SpriteComponent::SpriteComponent(const sol::table& table)
	{
		m_spriteName = table.get<std::string>("spriteName");
		m_opacity = table.get<float>("opacity");
	}

	SpriteComponent::SpriteComponent(const std::string& spriteName, float opacity)
		:m_spriteName(spriteName), m_opacity(opacity)
	{
	}

	SpriteComponent& SpriteComponent::operator=(const SpriteComponent&)
	{
		return *this;
	}
}