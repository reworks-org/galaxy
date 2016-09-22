//
//  Time.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TIME_HPP_
#define RENGINE3_TIME_HPP_

#include <ctime>
#include <chrono>
#include <string>
#include <algorithm>

#include "re/types/Types.hpp"

namespace re
{
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Get the current time and data in a std::string.
	*/
	inline std::string GetCurrentTimeAndDate()
	{
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp = std::ctime(&time);

		// Remove pesky newlines
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		return temp;
	}
}

#endif