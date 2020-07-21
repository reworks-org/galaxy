///
/// Time.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "Time.hpp"

#include <algorithm>
#include <ctime>
#include <string>

///
/// Core namespace.
///
namespace pr
{
	const double getTimeSinceEpoch(const std::chrono::high_resolution_clock::time_point* tp) noexcept
	{
		// Return time since epoch using chrono.
		return std::chrono::duration<double>(
			   (tp != nullptr ? *tp : std::chrono::high_resolution_clock::now()).time_since_epoch())
		    .count();
	}

	decltype(auto) getCurrentDateTime()
	{
		// Get current time using chrono.
		const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp       = std::ctime(&time);

		// Remove pesky newlines.
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		return std::move(temp);
	}

	decltype(auto) getShortTime()
	{
		// Get current time using chrono.
		const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp       = std::ctime(&time);

		// Remove pesky newlines.
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		temp.erase(temp.length() - 8);

		// Append PM or AM.
		if (std::stoi(temp.substr(11, 2)) >= 12)
		{
			temp.append(" PM");
		}
		else
		{
			temp.append(" AM");
		}

		temp = temp.substr(11, 8);

		return std::move(temp);
	}

	decltype(auto) getFormattedTime()
	{
		// Get current time using chrono.
		const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string temp       = std::ctime(&time);

		// Remove pesky newlines.
		temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

		// Remove special characters.
		temp.erase(std::remove(temp.begin(), temp.end(), ' '), temp.end());
		temp.erase(std::remove(temp.begin(), temp.end(), ':'), temp.end());

		// Trim string.
		temp = temp.substr(0, 12);

		return std::move(temp);
	}
} // namespace pr