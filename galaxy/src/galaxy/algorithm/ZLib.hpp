///
/// ZLib.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ALGORITHM_ZLIB_HPP_
#define GALAXY_ALGORITHM_ZLIB_HPP_

#include <string>

namespace galaxy
{
	namespace algorithm
	{
		///
		/// Compresses string into ZLib.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string encode_zlib(const std::string& input);

		///
		/// Decompresses string into ZLib.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string decode_zlib(const std::string& input);
	} // namespace algorithm
} // namespace galaxy

#endif