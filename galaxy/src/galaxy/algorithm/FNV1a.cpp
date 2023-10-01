///
/// FNV1a.cpp
/// galaxy
///
/// License: public domain or equivalent
/// Post: https://notes.underscorediscovery.com/constexpr-fnv1a/
/// Modified for use in Galaxy Engine.
///

#include "FNV1a.hpp"

namespace galaxy
{
	namespace algorithm
	{
		constexpr uint32_t fnv1a_32(const char* const str, const uint32_t value) noexcept
		{
			return (str[0] == '\0') ? value : fnv1a_32(&str[1], (value ^ uint32_t((uint8_t)str[0])) * prime_32_const);
		}

		constexpr uint64_t fnv1a_64(const char* const str, const uint64_t value) noexcept
		{
			return (str[0] == '\0') ? value : fnv1a_64(&str[1], (value ^ uint64_t((uint8_t)str[0])) * prime_64_const);
		}
	} // namespace algorithm
} // namespace galaxy