//
//  Time.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TIME_HPP_
#define RENGINE3_TIME_HPP_

#include <string>

#include "re/Types.hpp"

namespace re
{
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Use the high resolution clock to get the time. Use this to get time difference in loops.
	*/
	static sf::Uint64 NanoTime();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Get the current system time in milliseconds.
	*/
	static sf::Uint64 MillisTime();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Get the current time and data in a std::string.
	*/
	static std::string GetCurrentTimeAndDate();
}

#endif