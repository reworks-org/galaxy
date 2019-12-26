///
/// Time.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <ctime>
#include <algorithm>

#include "Time.hpp"

///
/// Core namespace.
///
namespace sc
{
	const std::uint32_t secondsToMilliseconds(const double seconds)
	{
		return static_cast<std::uint32_t>(seconds * 1000.0);
	}

	const std::uint64_t secondsToMicroseconds(const double seconds)
	{
		return static_cast<std::uint64_t>(seconds * 1000000.0);
	}

	const double millisecondsToSeconds(const std::uint32_t milliseconds)
	{
		return static_cast<double>(milliseconds) / 1000.0;
	}

	const std::uint64_t millisecondsToMicroseconds(const std::uint32_t milliseconds)
	{
		return static_cast<std::uint64_t>(static_cast<std::uint64_t>(milliseconds) * 1000);
	}

	const double microsecondsToSeconds(const std::uint64_t microseconds)
	{
		return static_cast<double>(microseconds / 1000000.0);
	}

	const std::uint32_t microsecondsToMilliseconds(const std::uint64_t microseconds)
	{
		return static_cast<std::uint32_t>(static_cast<double>(microseconds) / 1000.0);
	} 

	const double getTimeSinceEpoch(const std::chrono::high_resolution_clock::time_point* tp)
	{
		// Return time since epoch using chrono.
		return std::chrono::duration<double>((tp != nullptr ? *tp : std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
	}

	const std::string getCurrentDateTime()
	{
		// Get current time using chrono.
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp = std::ctime(&time);

		// Remove pesky newlines.
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		return temp;
	}

	const std::string getShortTime()
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

	const std::string getFormattedTime()
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