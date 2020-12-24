///
/// Algorithm.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <zlib.h>

#include "galaxy/error/Log.hpp"

#include "Algorithm.hpp"

namespace galaxy
{
	namespace algorithm
	{
		std::string decode_base64(const std::string& base64)
		{
			// Setup output.
			std::string out;

			if (!base64.empty())
			{
				// Base64 decode algorithm from:
				// https://stackoverflow.com/a/34571089
				std::vector<int> T(256, -1);
				for (int i = 0; i < 64; i++)
					T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

				int val  = 0;
				int valb = -8;
				for (auto c : base64)
				{
					if (T[c] == -1)
						break;
					val = (val << 6) + T[c];
					valb += 6;
					if (valb >= 0)
					{
						out.push_back(static_cast<char>((val >> valb) & 0xFF));
						valb -= 8;
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to decode empty base64 string.");
			}

			return out;
		}

		std::string decode_zlib(const std::string& zlib)
		{
			std::string out;

			if (!zlib.empty())
			{
				if (zlib.size() > 32768)
				{
					GALAXY_LOG(GALAXY_FATAL, "zlib input string too large.");
				}
				else
				{
					// Prep stream for decompressing.
					z_stream decompress;
					int err = 0;

					std::string zlib_out(32768, '\0');
					memset(&decompress, 0, sizeof(decompress));

					err = inflateInit(&decompress);
					if (err != Z_OK)
					{
						GALAXY_LOG(GALAXY_FATAL, "inflateInit failed! errcode: " + std::to_string(err));
					}
					else
					{
						decompress.next_in  = (Bytef*)zlib_out.data();
						decompress.avail_in = static_cast<uInt>(zlib.size());

						do
						{
							decompress.next_out  = reinterpret_cast<Bytef*>(zlib_out.data());
							decompress.avail_out = static_cast<uInt>(zlib_out.size());

							err = inflate(&decompress, 0);

							if (out.size() < decompress.total_out)
							{
								out.append(zlib_out,
									   decompress.total_out - out.size());
							}

						} while (err == Z_OK);
					}

					inflateEnd(&decompress);

					if (err < Z_OK)
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to inflate zlib data: {0}.", decompress.msg);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to decode empty zlib string.");
			}

			return out;
		}

		std::string decode_gzip(const std::string& gzip)
		{
			std::string out;

			if (!gzip.empty())
			{
				if (gzip.size() > 32768)
				{
					GALAXY_LOG(GALAXY_FATAL, "gzip input string too large.");
				}
				else
				{
					// Prep stream for decompressing.
					int err = 0;
					z_stream decompress;

					std::string gzip_out(32768, '\0');
					memset(&decompress, 0, sizeof(decompress));

					err = inflateInit2(&decompress, 16 + MAX_WBITS);
					if (err != Z_OK)
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to inflate gzip: {0}.", err);
					}
					else
					{
						decompress.next_in  = (Bytef*)gzip.data();
						decompress.avail_in = static_cast<uInt>(gzip.size());

						do
						{
							decompress.next_out  = reinterpret_cast<Bytef*>(gzip_out.data());
							decompress.avail_out = sizeof(gzip_out);

							err = inflate(&decompress, 0);

							if (out.size() < decompress.total_out)
							{
								out.append(gzip_out,
									   decompress.total_out - out.size());
							}

						} while (err == Z_OK);
					}

					inflateEnd(&decompress);

					if ((err != Z_OK) && (err != Z_STREAM_END))
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to inflate gzip data: {0}.", decompress.msg);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to decode empty gzip string.");
			}

			return out;
		}
	} // namespace algorithm
} // namespace galaxy