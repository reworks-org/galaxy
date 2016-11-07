//
//  System.cpp
//  REngine3
//
//  Created by reworks on 27/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "System.hpp"

namespace re
{
	System::~System()
	{
	}

	std::map<std::string, Entity*>& System::GetEntitys()
	{
		return m_entitys;
	}
}