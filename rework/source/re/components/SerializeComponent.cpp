//
//  SerializeComponent.cpp
//  rework
//
//  Created by reworks on 21/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "entityx/Entity.h"

#include "SerializeComponent.hpp"

namespace re
{
	SerializeComponent::SerializeComponent(ex::Entity& e, sol::table& table)
	{
		m_id = table.get<std::string>("id");
	}

	SerializeComponent::~SerializeComponent()
	{
		m_id = "";
	}
}