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
            // Setup output.
            std::string out = "";

            // Base64 decode algorithm from:
            // https://stackoverflow.com/a/34571089
            std::vector<int> T(256, -1);
            for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

            int val = 0, valb = -8;
            for (auto c : base64In) {
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
                z_stream decompress;
               
                char zlibOut[32768];
                memset(&decompress, 0, sizeof(decompress));
                memset(&zlibOut, 0, sizeof(zlibOut));
                
                err = inflateInit(&decompress);
                if (err != Z_OK)
                {
                    throw std::runtime_error("inflateInit failed! errcode: " + err);
                }
                else
                {
                    decompress.next_in = (Bytef*)zlibIn.data();
                    decompress.avail_in = zlibIn.size();

                    do {
                        decompress.next_out = reinterpret_cast<Bytef*>(zlibOut);
                        decompress.avail_out = sizeof(zlibOut);

                        err = inflate(&decompress, 0);

                        if (out.size() < decompress.total_out) {
                            out.append(zlibOut,
                                decompress.total_out - out.size());
                        }

                    } while (err == Z_OK);
                }

                inflateEnd(&decompress);

                if (err < Z_OK)
                {
                    throw std::runtime_error("Failed to inflate zlib data: " + std::string(decompress.msg));
                }
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
                z_stream decompress;

                char gzipOut[32768];
                memset(&decompress, 0, sizeof(decompress));
                memset(&gzipOut, 0, sizeof(gzipOut));

                err = inflateInit2(&decompress, 16 + MAX_WBITS);
                if (err != Z_OK)
                {
                    throw std::runtime_error("inflateInit failed! errcode: " + err);
                }
                else
                {
                    decompress.next_in = (Bytef*)gzipIn.data();
                    decompress.avail_in = gzipIn.size();

                    do {
                        decompress.next_out = reinterpret_cast<Bytef*>(gzipOut);
                        decompress.avail_out = sizeof(gzipOut);

                        err = inflate(&decompress, 0);

                        if (out.size() < decompress.total_out) {
                            out.append(gzipOut,
                                decompress.total_out - out.size());
                        }

                    } while (err == Z_OK);
                }

                inflateEnd(&decompress);

                if (err != Z_OK || err != Z_STREAM_END)
                {
                    throw std::runtime_error("Failed to inflate zlib data: " + std::string(decompress.msg));
                }
            }

            return out;
        }
	}
}