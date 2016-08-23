//
//  EventSystem.hpp
//  REngine3
//
//  Created by reworks on 23/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENTSYSTEM_HPP_
#define RENGINE3_EVENTSYSTEM_HPP_

#include "re/systems/System.hpp"

namespace re
{
	class EventSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the events.
		*/
		~EventSystem() override;
	};
}

#endif