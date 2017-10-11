//
//  CollisionComponent.cpp
//  rework
//
//  Created by reworks on 10/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "CollisionComponent.hpp"

namespace re
{
	CollisionComponent::CollisionComponent(sol::table& table)
	{
		m_width = table.get<int>("width");
		m_height = table.get<int>("height");
	}

	CollisionComponent::~CollisionComponent()
	{
	}
}