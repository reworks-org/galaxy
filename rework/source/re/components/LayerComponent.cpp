//
//  LayerComponent.cpp
//  rework
//
//  Created by reworks on 17/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "LayerComponent.hpp"

namespace re
{
	LayerComponent::LayerComponent(sol::table& table)
	{
		m_layer = table.get<std::size_t>("layer");
	}
	LayerComponent::~LayerComponent()
	{
	}
	void LayerComponent::debug()
	{
	}
}