///
/// FNV1a.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_FNV1A_HPP_
#define GALAXY_MATH_FNV1A_HPP_

#include <type_traits>

namespace galaxy
{
	namespace math
	{
		///
		/// Concept to enforce fnv bit types.
		///
		/// \tparam T Type to validate.
		///
		template<typename T>
		concept valid_fnv_bits = std::is_same_v<T, std::uint32_t> || std::is_same_v<T, std::uint64_t>;

		///
		/// Base specialization for fnv1a params.
		///
		/// \tparam bits std::uint32_t or std::uint64_t.
		///
		template<valid_fnv_bits bits = std::uint64_t>
		struct fnv_1a_params;

		///
		/// Fnv1a 32 bit specialization.
		///
		template<>
		struct fnv_1a_params<std::uint32_t>
		{
			static constexpr auto offset = 2166136261;
			static constexpr auto prime  = 16777619;
		};

		///
		/// Fnv1a 64 bit specialization.
		///
		template<>
		struct fnv_1a_params<std::uint64_t>
		{
			static constexpr auto offset = 14695981039346656037ull;
			static constexpr auto prime  = 1099511628211ull;
		};

		///
		/// Convert string to hash.
		///
		/// \tparam bits 32 or 64 bit type.
		///
		/// \param str String to hash.
		/// \param value Hashing value passed recursively.
		///
		/// \return Unsigned 32 or 64 bit integer.
		///
		template<valid_fnv_bits bits = std::uint64_t>
		inline constexpr bits fnv1a(const char* const str, const bits value = fnv_1a_params<bits>::offset) noexcept
		{
			return (str[0] == '\0') ? fnv_1a_params<bits>::offset : fnv1a<bits>(&str[1], (fnv_1a_params<bits>::offset ^ static_cast<bits>(str[0])) * fnv_1a_params<bits>::prime);
		}
	} // namespace math
} // namespace galaxy

#endif
