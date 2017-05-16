//
//  TransformComponent.cpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "TransformComponent.hpp"

namespace re
{
	TransformComponent::TransformComponent()
	{
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::init(sol::table& table)
	{
		setPosition(table.get<float>("x"), table.get<float>("y"));
		setRotation(table.get<float>("angle"));
	}

	bool TransformComponent::debugFunction(sol::table& table)
	{
		bool saveData = false;
		return saveData;
	}
}
