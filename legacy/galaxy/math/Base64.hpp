///
/// Base64.hpp
/// galaxy
///
/// Modified for use in Galaxy Engine.
///

#ifndef GALAXY_MATH_BASE64_HPP_
#define GALAXY_MATH_BASE64_HPP_

#include <string>

namespace galaxy
{
	namespace math
	{
		///
		/// Compresses string into Base64.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string encode_base64(const std::string& input);

		///
		/// Decompresses string into Base64.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string decode_base64(const std::string& input);
	} // namespace math
} // namespace galaxy

#endif