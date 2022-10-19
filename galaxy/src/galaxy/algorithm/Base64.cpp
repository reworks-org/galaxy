///
/// Base64.cpp
/// galaxy
///
/// Modified for use in Galaxy Engine.
///

/**
 * The MIT License (MIT)
 * Copyright (c) 2016 tomykaira
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "galaxy/error/Log.hpp"

#include "Base64.hpp"

// clang-format off
constexpr const char encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
    'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

constexpr const unsigned char decoding_table[] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63, 52, 53, 54, 55, 56, 57,
    58, 59, 60, 61, 64, 64, 64, 64, 64, 64, 64, 0, 1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64,
    64, 64, 64, 64, 64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64
};
// clang-format on

namespace galaxy
{
	namespace algorithm
	{
		std::string encode_base64(const std::string& input)
		{
			if (!input.empty())
			{
				const auto in_len  = input.size();
				const auto out_len = static_cast<std::size_t>(4 * ((in_len + 2) / 3));

				auto output = std::string(out_len, '\0');
				char* p     = &output[0];

				std::size_t i = 0;
				for (i = 0; i < in_len - 2; i += 3)
				{
					*p++ = encoding_table[(input[i] >> 2) & 0x3F];
					*p++ = encoding_table[((input[i] & 0x3) << 4) | (static_cast<int>(input[i + 1] & 0xF0) >> 4)];
					*p++ = encoding_table[((input[i + 1] & 0xF) << 2) | (static_cast<int>(input[i + 2] & 0xC0) >> 6)];
					*p++ = encoding_table[input[i + 2] & 0x3F];
				}

				if (i < in_len)
				{
					*p++ = encoding_table[(input[i] >> 2) & 0x3F];

					if (i == (in_len - 1))
					{
						*p++ = encoding_table[((input[i] & 0x3) << 4)];
						*p++ = '=';
					}
					else
					{
						*p++ = encoding_table[((input[i] & 0x3) << 4) | (static_cast<int>(input[i + 1] & 0xF0) >> 4)];
						*p++ = encoding_table[((input[i + 1] & 0xF) << 2)];
					}

					*p++ = '=';
				}

				return output;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to encode an empty string.");
				return {};
			}
		}

		std::string decode_base64(const std::string& input)
		{
			if (!input.empty())
			{
				const auto in_len = input.size();

				if (in_len % 4 != 0)
				{
					GALAXY_LOG(GALAXY_ERROR, "Input data size is not a multiple of 4");
					return {};
				}
				else
				{
					auto out_len = static_cast<std::size_t>(in_len / 4 * 3);

					if (input[in_len - 1] == '=')
					{
						out_len--;
					}

					if (input[in_len - 2] == '=')
					{
						out_len--;
					}

					auto output = std::string(out_len, '\0');

					for (std::size_t i = 0, j = 0; i < in_len;)
					{
						const std::uint32_t a = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];
						const std::uint32_t b = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];
						const std::uint32_t c = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];
						const std::uint32_t d = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];

						const std::uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

						if (j < out_len)
						{
							output[j++] = (triple >> 2 * 8) & 0xFF;
						}

						if (j < out_len)
						{
							output[j++] = (triple >> 1 * 8) & 0xFF;
						}

						if (j < out_len)
						{
							output[j++] = (triple >> 0 * 8) & 0xFF;
						}
					}

					return output;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to decode an empty string.");
				return {};
			}
		}
	} // namespace algorithm
} // namespace galaxy