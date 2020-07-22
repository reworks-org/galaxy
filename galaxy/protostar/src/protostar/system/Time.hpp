///
/// Time.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TIME_HPP_
#define PROTOSTAR_TIME_HPP_

#include <cstdint>

#include "protostar/system/Concepts.hpp"

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
	[[nodiscard]] inline constexpr const std::uint32_t seconds_to_milliseconds(not_negative auto seconds) noexcept
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
	[[nodiscard]] inline constexpr const std::uint64_t seconds_to_microseconds(not_negative auto seconds) noexcept
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
	[[nodiscard]] inline constexpr const double milliseconds_to_seconds(not_negative auto milliseconds) noexcept
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
	[[nodiscard]] inline constexpr const std::uint64_t
	milliseconds_to_microseconds(not_negative auto milliseconds) noexcept
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
	[[nodiscard]] inline constexpr const double microseconds_to_seconds(not_negative auto microseconds) noexcept
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
	[[nodiscard]] inline constexpr const std::uint32_t
	microseconds_to_milliseconds(not_negative auto microseconds) noexcept
	{
		return static_cast<std::uint32_t>(static_cast<double>(microseconds) / 1000.0);
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