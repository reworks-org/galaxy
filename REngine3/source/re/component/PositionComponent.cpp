//
//  PositionComponent.cpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "PositionComponent.hpp"

namespace re
{
	PositionComponent::PositionComponent(sol::table& table)
	{
		m_xpos = table.get<float>("x");
		m_ypos = table.get<float>("y");

		m_width = table.get<float>("w");
		m_height = table.get<float>("h");
	}

	PositionComponent::~PositionComponent()
	{
	}

	void PositionComponent::Update(float x, float y)
	{
		m_xpos = x;
		m_ypos = y;
	}
}