///
/// Time.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TIME_HPP_
#define PROTOSTAR_TIME_HPP_

#include <ctime>
#include <string>
#include <chrono>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// Convert seconds to milliseconds.
	///
	/// \param seconds Seconds to convert.
	///
	/// \return Returns const std::uint32_t.
	///
	const std::uint32_t secondsToMilliseconds(const double seconds) noexcept;

	///
	/// Convert seconds to microseconds.
	///
	/// \param seconds Seconds to convert.
	///
	/// \return Returns const std::uint64_t.
	///
	const std::uint64_t secondsToMicroseconds(const double seconds) noexcept;

	///
	/// Convert milliseconds to seconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const double.
	///
	const double millisecondsToSeconds(const std::uint32_t milliseconds) noexcept;

	///
	/// Convert milliseconds to microseconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const std::uint64_t.
	///
	const std::uint64_t millisecondsToMicroseconds(const std::uint32_t milliseconds) noexcept;

	///
	/// Convert microseconds to seconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns const double.
	///
	const double microsecondsToSeconds(const std::uint64_t microseconds) noexcept;

	///
	/// Convert microseconds to milliseconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns std::uint32_t.
	///
	const std::uint32_t microsecondsToMilliseconds(const std::uint64_t microseconds) noexcept;

	///
	/// \brief Returns High-Precision count of time passed since Epoch (Jan 1st, 1970).
	///
	/// Credits:
	/// https://stackoverflow.com/a/41580187
	///
	/// \param tp Another point in time to use.
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
	const std::string getCurrentDateTime() noexcept;

	///
	/// \brief Get the current time.
	///
	/// Time and Date in XX:YY AM/PM format.
	///
	/// \return Returns std::string shortened Time.
	///
	const std::string getShortTime() noexcept;

	///
	/// Get a formatted time that contains no special characters.
	///
	/// \return Returns std::string formatted Time.
	///
	const std::string getFormattedTime() noexcept;
}

#endif