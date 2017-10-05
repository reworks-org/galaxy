//
//  Time.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_TIME_HPP_
#define RENGINE3_TIME_HPP_

#include <ctime>
#include <chrono>
#include <string>
#include <algorithm>

#define SECONDS_TO_MILLISECONDS(s) (s * 1000.0)
#define SECONDS_TO_MICROSECONDS(s) (s * 1000000.0)

#define MILLISECONDS_TO_SECONDS(ms) (ms / 1000.0)
#define MILLISECONDS_TO_MICROSECONDS(ms) (ms * 1000.0)

#define MICROSECONDS_TO_SECONDS(us) (us / 1000000.0)
#define MICROSECONDS_TO_MILLISECONDS(us) (us / 1000.0)

#define SECONDS(s) (s)
#define MILLISECONDS(ms) (MILLISECONDS_TO_SECONDS(ms))
#define MICROSECONDS(us) (MICROSECONDS_TO_SECONDS(us))

namespace re
{
	namespace Time
	{
		///
		/// \brief Get the current time and date.
		///
		///  In XX:YY DD/MM/YYYY format.
		///
		/// \return Returns std::string.
		///
		inline std::string getCurrentDateTime()
		{
			std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::string temp = std::ctime(&time);

			// Remove pesky newlines
			temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

			return temp;
		}

		///
		/// \brief Get the current time.
		///
		///  Time and Date in XX:YY AM/PM format.
		///
		/// \return Returns std::string.
		///
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

		///
		/// Get a formatted time that contains no special characters.
		///
		/// \return Returns std::string.
		///
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