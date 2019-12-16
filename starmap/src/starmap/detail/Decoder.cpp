///
/// Decoder.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>
#include <stdexcept>

#include "zlib.h"

#include "Decoder.hpp"

namespace starmap
{
	namespace decoder
	{
		std::string base64(const std::string& base64In) noexcept
		{
            // See: https://stackoverflow.com/a/34571089.

            // Set up variables to use in decoding.
            std::string out;
            std::vector<int> T(256, -1);
            int val = 0;
            int valb = -8;

            for (int i = 0; i < 64; i++)
            {
                T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
            }

            for (auto& c : base64In) {
                if (T[c] == -1) break;
                val = (val << 6) + T[c];
                valb += 6;
                if (valb >= 0) {
                    out.push_back(char((val >> valb) & 0xFF));
                    valb -= 8;
                }
            }

            return out;
		}

        std::string zlib(const std::string& zlibIn)
        {
            // Output and error code.
            std::string out = "";
            int err = 0;

            if (zlibIn.size() > 32768)
            {
                throw std::runtime_error("zlib input string too large! errcode: " + err);
            }
            else
            {
                // Prep stream for decompressing.
                z_stream zs;
                char buff[32768];

                memset(&zs, 0, sizeof(zs));
                err = inflateInit(&zs);
                if (err != Z_OK)
                {
                    throw std::runtime_error("inflateInit failed! errcode: " + err);
                }
                else
                {
                    zs.next_in = (Bytef*)zlibIn.data();
                    zs.avail_in = zlibIn.size();

                    do
                    {
                        zs.next_out = reinterpret_cast<Bytef*>(buff);
                        zs.avail_out = sizeof(buff);
                        err = inflate(&zs, Z_NO_FLUSH);
                        if (out.size() < zs.total_out)
                        {
                            out.append(buff, zs.total_out - out.size());
                        }
                    } while (err == Z_OK);
                }

                inflateEnd(&zs);
            }
            
            return out;
        }

        std::string gzip(const std::string& gzipIn)
        {
            // Output and error code.
            std::string out = "";
            int err = 0;

            if (gzipIn.size() > 32768)
            {
                throw std::runtime_error("zlib input string too large! errcode: " + err);
            }
            else
            {
                // Prep stream for decompressing.
                z_stream zs;
                char buff[32768];

                memset(&zs, 0, sizeof(zs));
                err = inflateInit2(&zs, 16 + MAX_WBITS);
                if (err != Z_OK)
                {
                    throw std::runtime_error("inflateInit failed! errcode: " + err);
                }
                else
                {
                    zs.next_in = (Bytef*)gzipIn.data();
                    zs.avail_in = gzipIn.size();

                    do
                    {
                        zs.next_out = reinterpret_cast<Bytef*>(buff);
                        zs.avail_out = sizeof(buff);
                        err = inflate(&zs, Z_NO_FLUSH);
                        if (out.size() < zs.total_out)
                        {
                            out.append(buff, zs.total_out - out.size());
                        }
                    } while (err == Z_OK);
                }

                inflateEnd(&zs);
            }

            return out;
        }
	}
}