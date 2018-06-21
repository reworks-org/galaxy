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

#include <string>
#include <chrono>

namespace sl
{
	///
	/// Contains Time-related static utility functions.
	///
	struct Time
	{
		///
		/// Convert seconds to milliseconds.
		///
		/// \param seconds Seconds to convert.
		///
		/// \return Returns const std::uint32_t.
		///
		static const std::uint32_t secondsToMilliseconds(double seconds);

		///
		/// Convert seconds to microseconds.
		///
		/// \param seconds Seconds to convert.
		///
		/// \return Returns const std::uint64_t.
		///
		static const std::uint64_t secondsToMicroseconds(double seconds);

		///
		/// Convert milliseconds to seconds.
		///
		/// \param milliseconds Milliseconds to convert.
		///
		/// \return Returns const double.
		///
		static const double millisecondsToSeconds(std::uint32_t milliseconds);

		///
		/// Convert milliseconds to microseconds.
		///
		/// \param milliseconds Milliseconds to convert.
		///
		/// \return Returns const std::uint64_t.
		///
		static const std::uint64_t millisecondsToMicroseconds(std::uint32_t milliseconds);

		///
		/// Convert microseconds to seconds.
		///
		/// \param microseconds Microseconds to convert.
		///
		/// \return Returns const double.
		///
		static const double microsecondsToSeconds(std::uint64_t microseconds);

		///
		/// Convert microseconds to milliseconds.
		///
		/// \param microseconds Microseconds to convert.
		///
		/// \return Returns std::uint32_t.
		///
		static const std::uint32_t microsecondsToMilliseconds(std::uint64_t microseconds);

		///
		/// \brief seconds function.
		///
		/// Use this when counting time with the main loop.
		///
		/// \param seconds Number of seconds to compare.
		///
		/// \return Returns const double Doesn't actually do anything because we just want a way to compare with information.
		///
		static const double seconds(double seconds);

		///
		/// \brief milliseconds function.
		///
		/// Use this when counting time with the main loop.
		///
		/// \param milliseconds Number of seconds to compare.
		///
		/// \return Returns const double Converts to seconds because thats what the main loop uses.
		///
		static const double milliseconds(std::uint32_t milliseconds);

		///
		/// \brief microseconds function.
		///
		/// Use this when counting time with the main loop.
		///
		/// \param microseconds Number of microseconds to compare.
		///
		/// \return Returns const double Converts to seconds because thats what the main loop uses.
		///
		static const double microseconds(std::uint64_t microseconds);

		///
		/// \brief Returns High-Precision count of time passed since Epoch (Jan 1st, 1970).
		///
		/// Thanks to stackoverflow:
		/// https://stackoverflow.com/a/41580187
		///
		/// \param t Another point in time to use.
		///
		/// \return const double High Precision time passed since epoch.
		///
		static const double getTimeSinceEpoch(std::chrono::high_resolution_clock::time_point* t = nullptr);

		///
		/// \brief Get the current time and date.
		///
		/// In XX:YY DD/MM/YYYY format.
		///
		/// \return Returns const std::string of the current DateTime.
		///
		static std::string getCurrentDateTime();

		///
		/// \brief Get the current time.
		///
		/// Time and Date in XX:YY AM/PM format.
		///
		/// \return Returns std::string shortened Time.
		///
		static std::string getShortTime();

		///
		/// Get a formatted time that contains no special characters.
		///
		/// \return Returns std::string formatted Time.
		///
		static std::string getFormattedTime();
	};
}

#endif