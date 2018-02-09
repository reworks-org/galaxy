///
/// Time.hpp
/// starlight
///
/// Created by reworks on 08/07/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TIME_HPP_
#define STARLIGHT_TIME_HPP_

#include <ctime>
#include <chrono>
#include <string>
#include <cstdint>
#include <algorithm>

namespace sl
{
	namespace time
	{
		///
		/// Convert seconds to milliseconds.
		///
		/// \param seconds Seconds to convert.
		///
		/// \return Returns 32bit integer.
		///
		inline constexpr std::uint32_t secondsToMilliseconds(double seconds)
		{
			return static_cast<std::uint32_t>(seconds * 1000.0);
		}

		///
		/// Convert seconds to microseconds.
		///
		/// \param seconds Seconds to convert.
		///
		/// \return Returns 64bit integer.
		///
		inline constexpr std::uint64_t secondsToMicroseconds(double seconds)
		{
			return static_cast<std::uint64_t>(seconds * 1000000.0);
		}

		///
		/// Convert milliseconds to seconds.
		///
		/// \param milliseconds Milliseconds to convert.
		///
		/// \return Returns float.
		///
		inline constexpr double millisecondsToSeconds(std::uint32_t milliseconds)
		{
			return static_cast<double>(milliseconds) / 1000.0;
		}

		///
		/// Convert milliseconds to microseconds.
		///
		/// \param milliseconds Milliseconds to convert.
		///
		/// \return Returns 64bit integer.
		///
		inline constexpr std::uint64_t millisecondsToMicroseconds(std::uint32_t milliseconds)
		{
			return static_cast<std::uint64_t>(milliseconds * 1000);
		}

		///
		/// Convert microseconds to seconds.
		///
		/// \param microseconds Microseconds to convert.
		///
		/// \return Returns float.
		///
		inline constexpr double microsecondsToSeconds(std::uint64_t microseconds)
		{
			return static_cast<double>(microseconds / 1000000.0);
		}

		///
		/// Convert microseconds to milliseconds.
		///
		/// \param microseconds Microseconds to convert.
		///
		/// \return Returns 32bit integer.
		///
		inline constexpr std::uint32_t microsecondsToMilliseconds(std::uint64_t microseconds)
		{
			return static_cast<std::uint32_t>(static_cast<double>(microseconds) / 1000.0);
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
		inline constexpr double seconds(double seconds)
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
		inline constexpr double milliseconds(std::uint32_t milliseconds)
		{
			return time::millisecondsToSeconds(milliseconds);
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
		inline constexpr double microseconds(std::uint64_t microseconds)
		{
			return time::microsecondsToSeconds(microseconds);
		}

		///
		/// \brief Get the current time and date.
		///
		/// In XX:YY DD/MM/YYYY format.
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
		/// Time and Date in XX:YY AM/PM format.
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