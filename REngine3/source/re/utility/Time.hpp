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

namespace re
{
	namespace Time
	{
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Get the current time and date in a std::string.
		*/
		inline std::string getCurrentTimeAndDate()
		{
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::string temp = std::ctime(&time);

			// Remove pesky newlines
			temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

			return temp;
		}

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Time and Date in XX::YY AM/PM format.
		*/
		inline std::string getShortTime()
		{
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::string temp = std::ctime(&time);

			// Remove pesky newlines
			temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

			temp.erase(temp.length() - 8);

			if (std::stoi(temp.substr(11, 2)) >= 12) {
				temp.append(" PM");
			}
			else {
				temp.append(" AM");
			}

			temp = temp.substr(11, 8);

			return temp;
		}

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Get a formatted time that contains no special characters.
		*/
		inline std::string getFormattedTime()
		{
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::string temp = std::ctime(&time);

			// Remove pesky newlines
			temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

			// Remove special characters
			temp.erase(std::remove(temp.begin(), temp.end(), ' '), temp.end());
			temp.erase(std::remove(temp.begin(), temp.end(), ':'), temp.end());

			// Trim string
			temp = temp.substr(0, 12);

			return temp;
		}
	}
}

#endif
