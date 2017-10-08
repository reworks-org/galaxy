//
//  Time.hpp
//  rework
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TIME_HPP_
#define REWORK_TIME_HPP_

#include <ctime>
#include <chrono>
#include <string>
#include <cstdint>
#include <algorithm>

namespace re
{
	namespace Time
	{
		///
		/// Convert seconds to milliseconds.
		///
		/// \param seconds Seconds to convert.
		///
		/// \return Returns 32bit integer.
		///
		inline constexpr std::int32_t secondsToMilliseconds(float seconds)
		{
			return milliseconds * 1000;
		}

		///
		/// Convert seconds to microseconds.
		///
		/// \param seconds Seconds to convert.
		///
		/// \return Returns 64bit integer.
		///
		inline constexpr std::int64_t secondsToMicroseconds(float seconds)
		{
			return seconds * 1000000;
		}

		///
		/// Convert milliseconds to seconds.
		///
		/// \param milliseconds Milliseconds to convert.
		///
		/// \return Returns float.
		///
		inline constexpr float millisecondsToSeconds(std::int32_t milliseconds)
		{
			return milliseconds / 1000.0f;
		}

		///
		/// Convert milliseconds to microseconds.
		///
		/// \param milliseconds Milliseconds to convert.
		///
		/// \return Returns 64bit integer.
		///
		inline constexpr std::int64_t millisecondsToMicroseconds(std::int32_t milliseconds)
		{
			return  milliseconds * 1000;
		}

		///
		/// Convert microseconds to seconds.
		///
		/// \param microseconds Microseconds to convert.
		///
		/// \return Returns float.
		///
		inline constexpr float microsecondsToSeconds(std::int64_t microseconds)
		{
			return microseconds / 1000000.0f;
		}

		///
		/// Convert microseconds to milliseconds.
		///
		/// \param microseconds Microseconds to convert.
		///
		/// \return Returns 32bit integer.
		///
		inline constexpr std::int32_t microsecondsToMilliseconds(std::int64_t microseconds)
		{
			return microseconds / 1000;
		}

		///
		/// \brief seconds function.
		///
		/// Use this when counting time with the main loop.
		/// E.g. \code{.cpp} if (counter > Time::seconds(3)) { /*do something */ } \endcode
		///
		/// \param seconds Number of seconds to compare.
		///
		/// \return Returns float. Doesn't actually do anything because we just want a way to compare with information.
		///
		inline constexpr float seconds(float seconds)
		{
			return seconds;
		}

		///
		/// \brief milliseconds function.
		///
		/// Use this when counting time with the main loop.
		/// E.g. \code{.cpp} if (counter > Time::milliseconds(1000)) { /*do something */ } \endcode
		///
		/// \param seconds Number of milliseconds to compare.
		///
		/// \return Returns float. Converts to seconds because thats what the main loop uses.
		///
		inline constexpr float milliseconds(std::int32_t milliseconds)
		{
			return Time::millisecondsToSeconds(milliseconds);
		}

		///
		/// \brief microseconds function.
		///
		/// Use this when counting time with the main loop.
		/// E.g. \code{.cpp} if (counter > Time::microseconds(1000000)) { /*do something */ } \endcode
		///
		/// \param seconds Number of microseconds to compare.
		///
		/// \return Returns float. Converts to seconds because thats what the main loop uses.
		///
		inline constexpr float microseconds(std::int64_t microseconds)
		{
			return Time::microsecondsToSeconds(microseconds);
		}

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