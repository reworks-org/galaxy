///
/// Algorithm.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

#include <base64.hpp>
#include <zlc/gzipcomplete.hpp>
#include <zlc/zlibcomplete.hpp>

#include "galaxy/error/Log.hpp"

#include "Algorithm.hpp"

namespace galaxy
{
	namespace algorithm
	{
		std::string encode_zlib(const std::string& to_compress)
		{
			zlibcomplete::ZLibCompressor compressor;

			try
			{
				char in[ZLIB_COMPLETE_CHUNK] = {0};
				unsigned int total_read      = 0;

				std::string result;
				std::stringstream sstream;
				sstream.str(to_compress);

				while (true)
				{
					sstream.read(in, ZLIB_COMPLETE_CHUNK);
					total_read = static_cast<unsigned int>(sstream.gcount());

					if (total_read != 0)
					{
						result += compressor.compress({in, total_read});
					}
					else
					{
						break;
					}
				}

				result += compressor.finish();
				return result;
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				return "";
			}
		}

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
				char in[ZLIB_COMPLETE_CHUNK] = {0};
				unsigned int total_read      = 0;

				std::string result;
				std::stringstream sstream;
				sstream.str(zlib);

				while (true)
				{
					sstream.read(in, ZLIB_COMPLETE_CHUNK);
					total_read = static_cast<unsigned int>(sstream.gcount());

					if (total_read != 0)
					{
						result += decompressor.decompress({in, total_read});
					}
					else
					{
						break;
					}
				}

				return result;
			}
			catch (const std::exception& e)
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
				char in[ZLIB_COMPLETE_CHUNK] = {0};
				unsigned int total_read      = 0;

				std::string result;
				std::stringstream sstream;
				sstream.str(gzip);

				while (true)
				{
					sstream.read(in, ZLIB_COMPLETE_CHUNK);
					total_read = static_cast<unsigned int>(sstream.gcount());

					if (total_read != 0)
					{
						result += decompressor.decompress({in, total_read});
					}
					else
					{
						break;
					}
				}

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