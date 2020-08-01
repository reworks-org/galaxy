///
/// Decoder.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_DECODER_HPP_
#define STARMAP_DECODER_HPP_

#include <string>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Decoder encapsulation namespace.
	///
	namespace decoder
	{
		///
		/// \brief Decodes from base64.
		///
		/// Thanks to: https://stackoverflow.com/a/34571089.
		///
		/// \param base64In Takes in a base64 encoded string.
		///
		/// \return Returns a decoded std::string.
		///
		[[nodiscard]] std::string base64(const std::string& base64);

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
		[[nodiscard]] std::string zlib(const std::string& zlib);

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
		[[nodiscard]] std::string gzip(const std::string& gzip);
	} // namespace decoder
} // namespace starmap

#endif