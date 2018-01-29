///
///  SpriteComponent.cpp
///  starlight
///
///  Created by reworks on 12/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "sol2/sol.hpp"

#include "SpriteComponent.hpp"

namespace sl
{
	SpriteComponent::SpriteComponent(const sol::table& table)
	{
		m_spriteName = entt::HashedString(table.get<const char*>("spriteName"));
	}
}