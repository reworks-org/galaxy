//
//  ServiceLocator.cpp
//  REngine3
//
//  Created by reworks on 17/07/2016.
//  Copyright © 2016 reworks. All rights reserved.
//

#include "ServiceLocator.hpp"

namespace re
{
	std::unordered_map<std::type_index, Service*> Locator::m_Services;

	Locator::~Locator()
	{
		m_Services.clear();
	}
}