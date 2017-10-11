//
//  TransformComponent.cpp
//  rework
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "TransformComponent.hpp"

namespace re
{
	TransformComponent::TransformComponent(sol::table& table)
	{
		m_x = table.get<float>("x");
		m_y = table.get<float>("y");
		m_angle = table.get<float>("angle");
	}

	TransformComponent::~TransformComponent()
	{
	}
}