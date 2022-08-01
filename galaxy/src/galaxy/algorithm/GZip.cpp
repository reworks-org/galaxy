///
/// GZip.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

#include <zlc/gzipcomplete.hpp>

#include "galaxy/error/Log.hpp"

#include "GZip.hpp"

namespace galaxy
{
	namespace algorithm
	{
		std::string encode_gzip(const std::string& input)
		{
			zlibcomplete::GZipCompressor compressor;
			char* in = new char[ZLIB_COMPLETE_CHUNK];

			try
			{
				std::memset(in, 0, sizeof(char) * ZLIB_COMPLETE_CHUNK);

				unsigned int total_read = 0;

				std::string result;
				std::stringstream sstream;
				sstream.str(input);

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

				delete[] in;
				return result;
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());

				delete[] in;
				return {};
			}
		}

		std::string decode_gzip(const std::string& input)
		{
			zlibcomplete::GZipDecompressor decompressor;
			char* in = new char[ZLIB_COMPLETE_CHUNK];

			try
			{
				std::memset(in, 0, sizeof(char) * ZLIB_COMPLETE_CHUNK);

				unsigned int total_read = 0;

				std::string result;
				std::stringstream sstream;
				sstream.str(input);

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

				delete[] in;
				return result;
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());

				delete[] in;
				return {};
			}
		}
	} // namespace algorithm
} // namespace galaxy