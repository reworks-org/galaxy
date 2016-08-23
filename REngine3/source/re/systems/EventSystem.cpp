//
//  EventSystem.cpp
//  REngine3
//
//  Created by reworks on 23/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "EventSystem.hpp"

namespace re
{
	EventSystem::~EventSystem()
	{
		m_attachedEntitys.clear();
	}
}