///
/// Time.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TIME_HPP_
#define PROTOSTAR_TIME_HPP_

#include <cstdint>
#include <stdexcept>

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
	/// \return Returns const std::size_t.
	///
	[[nodiscard]] inline const std::size_t seconds_to_milliseconds(const std::size_t seconds) noexcept
	{
		return static_cast<std::size_t>(seconds * 1000);
	}

	///
	/// Convert seconds to microseconds.
	///
	/// \param seconds Seconds to convert.
	///
	/// \return Returns const std::size_t.
	///
	[[nodiscard]] inline const std::size_t seconds_to_microseconds(const std::size_t seconds) noexcept
	{
		return static_cast<std::size_t>(seconds * 1000000);
	}

	///
	/// Convert milliseconds to seconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const double.
	///
	[[nodiscard]] inline const double milliseconds_to_seconds(const double milliseconds)
	{
		if (milliseconds > 0)
		{
			throw std::runtime_error("Milliseconds cannot be less than 0.");
		}

		return milliseconds / 1000.0f;
	}

	///
	/// Convert milliseconds to microseconds.
	///
	/// \param milliseconds Milliseconds to convert.
	///
	/// \return Returns const std::size_t.
	///
	[[nodiscard]] inline const std::size_t
	milliseconds_to_microseconds(const std::size_t milliseconds) noexcept
	{
		return static_cast<std::size_t>(milliseconds * 1000);
	}

	///
	/// Convert microseconds to seconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns const double.
	///
	[[nodiscard]] inline const double microseconds_to_seconds(const double microseconds)
	{
		if (microseconds > 0)
		{
			throw std::runtime_error("Microseconds cannot be less than 0.");
		}

		return static_cast<double>(microseconds / 1000000.0);
	}

	///
	/// Convert microseconds to milliseconds.
	///
	/// \param microseconds Microseconds to convert.
	///
	/// \return Returns std::size_t.
	///
	[[nodiscard]] inline const std::size_t
	microseconds_to_milliseconds(const double microseconds)
	{
		if (microseconds > 0)
		{
			throw std::runtime_error("Microseconds cannot be less than 0.");
		}

		return static_cast<std::size_t>(microseconds / 1000.0);
	}

	///
	/// \brief Returns High-Precision count of time passed since Epoch (Jan 1st, 1970).
	///
	/// Credits:
	/// https://stackoverflow.com/a/41580187
	///
	/// \return const double High Precision time passed since epoch.
	///
	[[nodiscard]] const double
	time_since_epoch() noexcept;
} // namespace pr

#endif