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
			auto in = new char[ZLIB_COMPLETE_CHUNK];

			std::string result;

			try
			{
				zlibcomplete::GZipCompressor compressor;
				std::memset(in, 0, sizeof(char) * ZLIB_COMPLETE_CHUNK);

				unsigned int total_read = 0;

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
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}

			delete[] in;
			return result;
		}

		std::string decode_gzip(const std::string& input)
		{
			auto in = new char[ZLIB_COMPLETE_CHUNK];

			std::string result;

			try
			{
				zlibcomplete::GZipDecompressor decompressor;
				std::memset(in, 0, sizeof(char) * ZLIB_COMPLETE_CHUNK);

				unsigned int total_read = 0;

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
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}

			delete[] in;
			return result;
		}
	} // namespace algorithm
} // namespace galaxy