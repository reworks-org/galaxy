///
/// ZLib.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

#include <zlc/zlibcomplete.hpp>

#include "galaxy/error/Log.hpp"

#include "ZLib.hpp"

namespace galaxy
{
	namespace algorithm
	{
		std::string encode_zlib(const std::string& input)
		{
			zlibcomplete::ZLibCompressor compressor;

			try
			{
				char in[ZLIB_COMPLETE_CHUNK] = {0};
				unsigned int total_read      = 0;

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
				return result;
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				return {};
			}
		}

		std::string decode_zlib(const std::string& input)
		{
			zlibcomplete::ZLibDecompressor decompressor;

			try
			{
				char in[ZLIB_COMPLETE_CHUNK] = {0};
				unsigned int total_read      = 0;

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

				return result;
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
				return {};
			}
		}
	} // namespace algorithm
} // namespace galaxy