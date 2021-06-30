///
/// GZip.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_GZIP_HPP_
#define GALAXY_MATH_GZIP_HPP_

#include <string>

namespace galaxy
{
	namespace math
	{
		///
		/// Compresses string into GZip.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string encode_gzip(const std::string& input);

		///
		/// Decompresses string into GZip.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string decode_gzip(const std::string& input);
	} // namespace math
} // namespace galaxy

#endif