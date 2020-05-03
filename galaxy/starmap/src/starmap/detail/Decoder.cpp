///
/// Decoder.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>
#include <stdexcept>

#include <zlib/zlib.h>
#include <pulsar/Log.hpp>

#include "Decoder.hpp"

namespace starmap
{
	namespace decoder
	{
		std::string base64(const std::string& base64In) noexcept
		{
            // Setup output.
            std::string out = "";

            if (!base64In.empty())
            {
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
            }
            else
            {
                PL_LOG(PL_ERROR, "Attempted to decode empty base64 string.");
            }

            return std::move(out);
        }

        std::string zlib(const std::string& zlibIn) noexcept
        {
            std::string out = "";

            if (!zlibIn.empty())
            {
                if (zlibIn.size() > 32768)
                {
                    PL_LOG(PL_FATAL, "zlib input string too large.");
                }
                else
                {
                    // Prep stream for decompressing.
                    z_stream decompress;
                    int err = 0;

                    char* zlibOut = new char[32768];
                    memset(&decompress, 0, sizeof(decompress));
                    memset(zlibOut, 0, 32768 * sizeof(char));

                    err = inflateInit(&decompress);
                    if (err != Z_OK)
                    {
                        delete[] zlibOut;
                        zlibOut = nullptr;
                        PL_LOG(PL_FATAL, "inflateInit failed! errcode: " + err);
                    }
                    else
                    {
                        decompress.next_in = (Bytef*)zlibIn.data();
                        decompress.avail_in = static_cast<uInt>(zlibIn.size());

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
                        if (zlibOut != nullptr)
                        {
                            delete[] zlibOut;
                            zlibOut = nullptr;
                        }

                        PL_LOG(PL_FATAL, "Failed to inflate zlib data: " + std::string(decompress.msg));
                    }

                    if (zlibOut != nullptr)
                    {
                        delete[] zlibOut;
                        zlibOut = nullptr;
                    }
                }
            }
            else
            {
                PL_LOG(PL_ERROR, "Attempted to decode empty zlib string.");
            }
            
            return std::move(out);
        }

        std::string gzip(const std::string& gzipIn) noexcept
        {
            std::string out = "";

            if (!gzipIn.empty())
            {
                if (gzipIn.size() > 32768)
                {
                    PL_LOG(PL_FATAL, "gzip input string too large.");
                }
                else
                {
                    // Prep stream for decompressing.
                    int err = 0;
                    z_stream decompress;

                    char* gzipOut = new char[32768];
                    memset(&decompress, 0, sizeof(decompress));
                    memset(gzipOut, 0, 32768 * sizeof(char));

                    err = inflateInit2(&decompress, 16 + MAX_WBITS);
                    if (err != Z_OK)
                    {
                        delete[] gzipOut;
                        gzipOut = nullptr;
                        PL_LOG(PL_FATAL, "inflateInit failed! errcode: " + err);
                    }
                    else
                    {
                        decompress.next_in = (Bytef*)gzipIn.data();
                        decompress.avail_in = static_cast<uInt>(gzipIn.size());

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

                    if ((err != Z_OK) && (err != Z_STREAM_END))
                    {
                        if (gzipOut != nullptr)
                        {
                            delete[] gzipOut;
                            gzipOut = nullptr;
                        }

                        PL_LOG(PL_FATAL, "Failed to inflate gzip data: " + std::string(decompress.msg));
                    }

                    if (gzipOut != nullptr)
                    {
                        delete[] gzipOut;
                        gzipOut = nullptr;
                    }
                }
            }
            else
            {
                PL_LOG(PL_ERROR, "Attempted to decode empty gzip string.");
            }

            return std::move(out);
        }
	}
}