//
//  Events.hpp
//  REngine3
//
//  Created by reworks on 5/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENTS_HPP_
#define RENGINE3_EVENTS_HPP_

namespace re
{
	// Just to make code look nicer.
	typedef int EventType;

	///
	/// List of event types. Inherit to define your own types. Don't forget to add the string equivalent to the converter map.
	/// 0-3 taken. Start at 4.
	///
	struct Event
	{
		static const int MOUSE_PRESSED = 0;
		static const int MOUSE_RELEASED = 1;
		static const int MOUSED_MOVED = 2;
		static const int PLAYER_ACTION = 3;
	};
}

#endif