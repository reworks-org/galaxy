///
/// Algorithm.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ALGORITHM_ALGORITHM_HPP_
#define GALAXY_ALGORITHM_ALGORITHM_HPP_

#include <optional>
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
		[[nodiscard]] inline float normalize(const Type val, const Type max)
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
		/// \brief Decodes from base64.
		///
		/// Thanks to: https://stackoverflow.com/a/34571089.
		///
		/// \param base64In Takes in a base64 encoded string.
		///
		/// \return Returns a decoded std::string.
		///
		[[nodiscard]] std::string decode_base64(const std::string& base64);

		///
		/// \brief Decompresses from zlib.
		///
		/// Note: Maximum size of: 32768 (1024^32) allowed.
		/// Note: Can throw exception.
		/// Thanks to: https://stackoverflow.com/a/50082615
		///
		/// \param zlibIn zlib compressed string.
		///
		/// \return Decompressed zlib string.
		///
		[[nodiscard]] std::string decode_zlib(const std::string& zlib);

		///
		/// \brief Decompresses from gzip.
		///
		/// Note: Maximum size of: 32768 (1024^32) allowed.
		/// Note: Can throw exception.
		/// Thanks to: https://stackoverflow.com/a/50082615
		///
		/// \param gzipIn gzip compressed string.
		///
		/// \return Decompressed gzip string.
		///
		[[nodiscard]] std::string decode_gzip(const std::string& gzip);
	} // namespace algorithm

} // namespace galaxy

#endif