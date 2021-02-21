///
/// Algorithm.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <base64.hpp>
#include <zlc/gzipcomplete.hpp>
#include <zlc/zlibcomplete.hpp>

#include "galaxy/error/Log.hpp"

#include "Algorithm.hpp"

namespace galaxy
{
	namespace algorithm
	{
		std::string decode_base64(const std::string& base64)
		{
			std::string output;
			if (!macaron::Base64::Decode(base64, output).empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to decode base64 string, is the input data a multiple of 4?");
				return "";
			}
			else
			{
				return output;
			}
		}

		std::string decode_zlib(const std::string& zlib)
		{
			zlibcomplete::ZLibDecompressor decompressor;
			try
			{
				const auto result = decompressor.decompress(zlib);
				return result;
			}
			catch (std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				return "";
			}
		}

		std::string decode_gzip(const std::string& gzip)
		{
			zlibcomplete::GZipDecompressor decompressor;
			try
			{
				const auto result = decompressor.decompress(gzip);
				return result;
			}
			catch (std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				return "";
			}
		}
	} // namespace algorithm
} // namespace galaxy