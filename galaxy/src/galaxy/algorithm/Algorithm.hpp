///
/// Algorithm.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ALGORITHM_ALGORITHM_HPP_
#define GALAXY_ALGORITHM_ALGORITHM_HPP_

#include <vector>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace algorithm
	{
		///
		/// Calc normalized value from range.
		///
		/// \param val Value to divide by max.
		/// \param max Value representing 1.0f.
		///
		/// \return Normalized float.
		///
		template<meta::is_arithmetic Type>
		[[nodiscard]] inline float normalize(const Type val, const Type max) noexcept
		{
			return (static_cast<float>(val) / static_cast<float>(max));
		}

		///
		/// See if a vector contains a value.
		///
		/// \param cont Container to check.
		/// \param val Value to look for.
		///
		/// \return Either iterator pointing to the end or to the value.
		///
		template<typename Type>
		[[nodiscard]] inline auto contains(const std::vector<Type>& cont, const Type val)
		{
			return std::find(cont.begin(), cont.end(), val);
		}

		///
		/// Decodes from base64.
		///
		/// \param base64 Takes in a base64 encoded string.
		///
		/// \return Returns a decoded std::string.
		///
		[[nodiscard]] std::string decode_base64(const std::string& base64);

		///
		/// Decompresses from zlib encoding.
		///
		/// \param zlib zlib compressed string.
		///
		/// \return Decompressed std::string.
		///
		[[nodiscard]] std::string decode_zlib(const std::string& zlib);

		///
		/// Decompresses from gzip encoding.
		///
		/// \param gzip gzip compressed string.
		///
		/// \return Decompressed std::string.
		///
		[[nodiscard]] std::string decode_gzip(const std::string& gzip);
	} // namespace algorithm

} // namespace galaxy

#endif