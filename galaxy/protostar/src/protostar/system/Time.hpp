///
/// Time.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TIME_HPP_
#define PROTOSTAR_TIME_HPP_

#include <string>
#include <chrono>

///
/// Core namespace.
///
namespace pr
{
	///
	/// Convert seconds to milliseconds.
	///
	/// \param seconds Seconds to convert.
	///
	/// \return Returns const std::uint32_t.
	///
	inline constexpr const std::uint32_t secondsToMilliseconds(const double seconds) noexcept
	{
		return static_cast<std::uint32_t>(seconds * 1000.0);
	}

	///
	/// Convert seconds to microseconds.
	///
	/// \param seconds Seconds to convert.
	///
	/// \return Returns const std::uint64_t.
	///
	inline constexpr const std::uint64_t secondsToMicroseconds(const double seconds) noexcept
	{
		return static_cast<std::uint64_t>(seconds * 1000000.0);
	}

	///
	/// Convert milliseconds to seconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const double.
	///
	inline constexpr const double millisecondsToSeconds(const std::uint32_t milliseconds) noexcept
	{
		return static_cast<double>(milliseconds) / 1000.0;
	}

	///
	/// Convert milliseconds to microseconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const std::uint64_t.
	///
	inline constexpr const std::uint64_t millisecondsToMicroseconds(const std::uint32_t milliseconds) noexcept
	{
		 return static_cast<std::uint64_t>(static_cast<std::uint64_t>(milliseconds) * 1000);
	}

	///
	/// Convert microseconds to seconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns const double.
	///
	inline constexpr const double microsecondsToSeconds(const std::uint64_t microseconds) noexcept
	{
		return static_cast<double>(microseconds / 1000000.0);
	}

	///
	/// Convert microseconds to milliseconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns std::uint32_t.
	///
	inline constexpr const std::uint32_t microsecondsToMilliseconds(const std::uint64_t microseconds) noexcept
	{
		return static_cast<std::uint32_t>(static_cast<double>(microseconds) / 1000.0);
	}

	///
	/// \brief Returns High-Precision count of time passed since Epoch (Jan 1st, 1970).
	///
	/// Credits:
	/// https://stackoverflow.com/a/41580187
	///
	/// \param tp Optional. Another point in time to use.
	///
	/// \return const double High Precision time passed since epoch.
	///
	const double getTimeSinceEpoch(const std::chrono::high_resolution_clock::time_point* tp = nullptr) noexcept;

	///
	/// \brief Get the current time and date.
	///
	/// In XX:YY DD/MM/YYYY format.
	///
	/// \return Returns const std::string of the current DateTime.
	///
	std::string&& getCurrentDateTime();

	///
	/// \brief Get the current time.
	///
	/// Time and Date in XX:YY AM/PM format.
	///
	/// \return Returns std::string shortened Time.
	///
	std::string&& getShortTime();

	///
	/// Get a formatted time that contains no special characters.
	///
	/// \return Returns std::string formatted Time.
	///
	std::string&& getFormattedTime();
}

#endif