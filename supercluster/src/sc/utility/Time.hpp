///
/// Time.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_TIME_HPP_
#define SUPERCLUSTER_TIME_HPP_

#include <string>
#include <chrono>

///
/// Core namespace.
///
namespace sc
{
	///
	/// Convert seconds to milliseconds.
	///
	/// \param seconds Seconds to convert.
	///
	/// \return Returns const std::uint32_t.
	///
	const std::uint32_t secondsToMilliseconds(const double seconds);

	///
	/// Convert seconds to microseconds.
	///
	/// \param seconds Seconds to convert.
	///
	/// \return Returns const std::uint64_t.
	///
	const std::uint64_t secondsToMicroseconds(const double seconds);

	///
	/// Convert milliseconds to seconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const double.
	///
	const double millisecondsToSeconds(const std::uint32_t milliseconds);

	///
	/// Convert milliseconds to microseconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const std::uint64_t.
	///
	const std::uint64_t millisecondsToMicroseconds(const std::uint32_t milliseconds);

	///
	/// Convert microseconds to seconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns const double.
	///
	const double microsecondsToSeconds(const std::uint64_t microseconds);

	///
	/// Convert microseconds to milliseconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns std::uint32_t.
	///
	const std::uint32_t microsecondsToMilliseconds(const std::uint64_t microseconds);

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
	const double getTimeSinceEpoch(const std::chrono::high_resolution_clock::time_point* tp = nullptr);

	///
	/// \brief Get the current time and date.
	///
	/// In XX:YY DD/MM/YYYY format.
	///
	/// \return Returns const std::string of the current DateTime.
	///
	const std::string getCurrentDateTime();

	///
	/// \brief Get the current time.
	///
	/// Time and Date in XX:YY AM/PM format.
	///
	/// \return Returns std::string shortened Time.
	///
	const std::string getShortTime();

	///
	/// Get a formatted time that contains no special characters.
	///
	/// \return Returns std::string formatted Time.
	///
	const std::string getFormattedTime();
}

#endif