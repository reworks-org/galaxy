///
/// Time.cpp
/// starlight
///
/// Created by reworks on 08/07/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <ctime>
#include <algorithm>

#include "Time.hpp"

namespace sl
{
	const std::uint32_t Time::secondsToMilliseconds(double seconds)
	{
		return static_cast<std::uint32_t>(seconds * 1000.0);
	}

	const std::uint64_t Time::secondsToMicroseconds(double seconds)
	{
		return static_cast<std::uint64_t>(seconds * 1000000.0);
	}

	const double Time::millisecondsToSeconds(std::uint32_t milliseconds)
	{
		return static_cast<double>(milliseconds) / 1000.0;
	}

	const std::uint64_t Time::millisecondsToMicroseconds(std::uint32_t milliseconds)
	{
		return static_cast<std::uint64_t>(milliseconds * 1000);
	}

	const double Time::microsecondsToSeconds(std::uint64_t microseconds)
	{
		return static_cast<double>(microseconds / 1000000.0);
	}

	const std::uint32_t Time::microsecondsToMilliseconds(std::uint64_t microseconds)
	{
		return static_cast<std::uint32_t>(static_cast<double>(microseconds) / 1000.0);
	}

	const double Time::seconds(double seconds)
	{
		return seconds;
	}

	const double Time::milliseconds(std::uint32_t milliseconds)
	{
		return Time::millisecondsToSeconds(milliseconds);
	}

	const double Time::microseconds(std::uint64_t microseconds)
	{
		return Time::microsecondsToSeconds(microseconds);
	}

	const double Time::getTimeSinceEpoch(std::chrono::high_resolution_clock::time_point* t)
	{
		// Shorten namespace.
		using clock = std::chrono::high_resolution_clock;

		// Return time since epoch using chrono.
		return std::chrono::duration<double>((t != nullptr ? *t : clock::now()).time_since_epoch()).count();
	}

	std::string Time::getCurrentDateTime()
	{
		// Get current time using chrono.
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp = std::ctime(&time);

		// Remove pesky newlines.
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		return temp;
	}

	std::string Time::getShortTime()
	{
		// Get current time using chrono.
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp = std::ctime(&time);

		// Remove pesky newlines.
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		temp.erase(temp.length() - 8);

		// Append PM or AM.
		if (std::stoi(temp.substr(11, 2)) >= 12) {
			temp.append(" PM");
		}
		else {
			temp.append(" AM");
		}

		temp = temp.substr(11, 8);

		return temp;
	}

	std::string Time::getFormattedTime()
	{
		// Get current time using chrono.
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp = std::ctime(&time);

		// Remove pesky newlines.
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		// Remove special characters.
		temp.erase(std::remove(temp.begin(), temp.end(), ' '), temp.end());
		temp.erase(std::remove(temp.begin(), temp.end(), ':'), temp.end());

		// Trim string.
		temp = temp.substr(0, 12);

		return temp;
	}
}