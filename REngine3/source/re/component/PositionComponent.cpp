//
//  PositionComponent.cpp
//  REngine3
//
//  Created by reworks on 1/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/deps/sol2/sol.hpp"

#include "PositionComponent.hpp"

namespace re
{
	PositionComponent::PositionComponent(sol::table& table)
	{
		m_group = table["group"];

		x = table["x"];
		y = table["y"];
		w = table["w"];
		h = table["h"];
	}

	PositionComponent::~PositionComponent()
	{
		x = y = w = h = 0;
	}
}