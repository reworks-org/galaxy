//
//  ServiceLocator.cpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "ServiceLocator.hpp"

namespace re
{
	std::unordered_map<std::type_index, Service*> Locator::m_services;

	Locator::~Locator()
	{
		m_services.clear();
	}
}