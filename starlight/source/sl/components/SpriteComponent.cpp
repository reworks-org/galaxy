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
	}

	SpriteComponent& SpriteComponent::operator=(const SpriteComponent &)
	{
		return *this;
	}
}