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
	PositionComponent::PositionComponent()
	{
	}

	PositionComponent::~PositionComponent()
	{
	}

	void PositionComponent::Init(sol::table& table)
	{
		m_xpos = table.get<float>("x");
		m_ypos = table.get<float>("y");

		m_width = table.get<float>("w");
		m_height = table.get<float>("h");
	}

	void PositionComponent::Update(sf::Time dt, float x, float y)
	{
	}

	void PositionComponent::SetPos(float x, float y)
	{
		m_xpos = x;
		m_ypos = y;
	}
}