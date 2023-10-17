///
/// StringUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <cctype>

#include "StringUtils.hpp"

namespace galaxy
{
	namespace strutils
	{
		meta::vector<std::string> split(std::string_view input, std::string_view delim)
		{
			std::size_t start = 0;
			std::size_t end   = 0;

			meta::vector<std::string> splits;
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

		void replace_first(std::string& input, std::string_view to_replace, std::string_view replace_with)
		{
			const auto pos = input.find(to_replace);

			if (pos != std::string::npos)
			{
				input.replace(pos, to_replace.length(), replace_with);
			}
		}

		void replace_all(std::string& input, std::string_view to_replace, std::string_view replace_with)
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
		}

		bool begins_with(const std::string& input, const std::string& find)
		{
			return (input.rfind(find, 0) == 0);
		}

		void ltrim(std::string& input)
		{
			input.erase(input.begin(), std::find_if(input.begin(), input.end(), [](const auto ch) {
				return !std::isspace(ch);
			}));
		}

		void rtrim(std::string& input)
		{
			// clang-format off
			input.erase(std::find_if(input.rbegin(), input.rend(), [](const auto ch) {
                return !std::isspace(ch);
			}).base(), input.end());
			// clang-format on
		}

		void trim(std::string& input)
		{
			rtrim(input);
			ltrim(input);
		}

		void make_single_spaced(std::string& input)
		{
			const auto trim_from = std::unique(input.begin(), input.end(), [](const auto lhs, const auto rhs) {
				return (lhs == rhs) && (lhs == ' ');
			});

			input.erase(trim_from, input.end());
			input.shrink_to_fit();
		}
	} // namespace strutils
} // namespace galaxy
