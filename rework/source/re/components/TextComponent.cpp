//
//  TextComponent.cpp
//  rework
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "TextComponent.hpp"

namespace re
{
	TextComponent::TextComponent(sol::table& table)
	{
		m_layer = table.get<unsigned int>("layer");


	}

	TextComponent::~TextComponent()
	{
	}
}