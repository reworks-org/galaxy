//
//  SerializeComponent.cpp
//  rework
//
//  Created by reworks on 21/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "SerializeComponent.hpp"

namespace re
{
	SerializeComponent::SerializeComponent(sol::table& table)
	{
		m_id = table.get<std::string>("id");
	}

	SerializeComponent::~SerializeComponent()
	{
	}

	void SerializeComponent::save(ex::Entity& e)
	{
	}

	void SerializeComponent::load(ex::Entity& e)
	{
	}
}