//
//  Event.hpp
//  REngine3
//
//  Created by reworks on 5/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENT_HPP_
#define RENGINE3_EVENT_HPP_

namespace re
{
	typedef int EventType;

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: List of event types. Inherit to define your own types.
	*/
	struct Event
	{
		static const int MOUSE_PRESSED = 0;
		static const int MOUSE_RELEASED = 1;
		static const int MOUSED_MOVED = 2;
		static const int PLAYER_ACTION = 3;
	};
}

#endif