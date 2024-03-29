///
/// FNV1a.hpp
/// galaxy
///
/// License: public domain or equivalent
/// Post: https://notes.underscorediscovery.com/constexpr-fnv1a/
/// Modified for use in Galaxy Engine.
///

#ifndef GALAXY_MATH_FNV1A_HPP_
#define GALAXY_MATH_FNV1A_HPP_

#include <cstdint>

namespace galaxy
{
	namespace math
	{
		///
		/// Hash value 32bit.
		///
		constexpr std::uint32_t val_32_const = 0x811c9dc5;

		///
		/// Prime value 32bit.
		///
		constexpr std::uint32_t prime_32_const = 0x1000193;

		///
		/// Hash value 64bit.
		///
		constexpr std::uint64_t val_64_const = 0xcbf29ce484222325;

		///
		/// Prime value 64bit.
		///
		constexpr std::uint64_t prime_64_const = 0x100000001b3;

		///
		/// Convert string to 32bit hash.
		///
		/// \param str String to hash.
		/// \param value Not required.
		///
		/// \return Unsigned 32bit integer.
		///
		constexpr inline std::uint32_t fnv1a_32(const char* const str, const std::uint32_t value = val_32_const) noexcept
		{
			return (str[0] == '\0') ? value : fnv1a_32(&str[1], (value ^ static_cast<uint32_t>(static_cast<uint8_t>(str[0]))) * prime_32_const);
		}

		///
		/// Convert string to 64bit hash.
		///
		/// \param str String to hash.
		/// \param value Not required.
		///
		/// \return Unsigned 64bit integer.
		///
		constexpr inline std::uint64_t fnv1a_64(const char* const str, const std::uint64_t value = val_64_const) noexcept
		{
			return (str[0] == '\0') ? value : fnv1a_64(&str[1], (value ^ static_cast<uint64_t>(static_cast<uint8_t>(str[0]))) * prime_64_const);
		}

	} // namespace math
} // namespace galaxy

#endif
