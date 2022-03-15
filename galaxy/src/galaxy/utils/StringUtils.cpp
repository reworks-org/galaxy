///
/// StringUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "StringUtils.hpp"

namespace galaxy
{
	namespace strutils
	{
		std::vector<std::string> split(std::string_view input, std::string_view delim) noexcept
		{
			std::size_t start = 0;
			std::size_t end   = 0;

			std::vector<std::string> splits;
			while ((start = input.find_first_not_of(delim, end)) != std::string::npos)
			{
				end = input.find(delim, start);

				const auto split = input.substr(start, end - start);
				if (!split.empty())
				{
					splits.emplace_back(split);
				}
			}

			return splits;
		}

		std::string parse_codepoint(const unsigned int codepoint) noexcept
		{
			// https://stackoverflow.com/a/19968992

			std::string out;

			if (codepoint <= 0x7f)
			{
				out.append(1, static_cast<char>(codepoint));
			}
			else if (codepoint <= 0x7ff)
			{
				out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
				out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
			}
			else if (codepoint <= 0xffff)
			{
				out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
				out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
				out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
			}
			else
			{
				out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
				out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
				out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
				out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
			}

			return out;
		}

		std::string& replace_first(std::string& input, std::string_view to_replace, std::string_view replace_with)
		{
			const auto pos = input.find(to_replace);

			if (pos != std::string::npos)
			{
				input.replace(pos, to_replace.length(), replace_with);
			}

			return input;
		}

		std::string& replace_all(std::string& input, std::string_view to_replace, std::string_view replace_with)
		{
			std::size_t pos = 0;

			while (pos != std::string::npos)
			{
				pos = input.find(to_replace, pos);

				if (pos != std::string::npos)
				{
					input.replace(pos, to_replace.length(), replace_with);
				}
			}

			return input;
		}
	} // namespace strutils
} // namespace galaxy