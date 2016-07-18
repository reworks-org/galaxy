//
//  Time.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <ctime>
#include <chrono>
#include <algorithm>

#include "Time.hpp"

namespace re
{
	sf::Uint64 NanoTime()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	sf::Uint64 MillisTime()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	std::string GetCurrentTimeAndDate()
	{
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp = std::ctime(&time);

		// Remove pesky newlines
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		return temp;
	}
}