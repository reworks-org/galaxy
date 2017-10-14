//
//  SpriteComponent.cpp
//  rework
//
//  Created by reworks on 10/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "SpriteComponent.hpp"

namespace re
{
	SpriteComponent::SpriteComponent(sol::table& table)
	{
		m_layer = table.get<unsigned int>("layer");
		m_spriteName = table.get<std::string>("spriteName");	
	}

	SpriteComponent::~SpriteComponent()
	{
	}
}
