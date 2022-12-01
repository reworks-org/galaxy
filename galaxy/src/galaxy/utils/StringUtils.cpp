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

		std::string& replace_first(std::string& input, std::string_view to_replace, std::string_view replace_with) noexcept
		{
			const auto pos = input.find(to_replace);

			if (pos != std::string::npos)
			{
				input.replace(pos, to_replace.length(), replace_with);
			}

			return input;
		}

		std::string& replace_all(std::string& input, std::string_view to_replace, std::string_view replace_with) noexcept
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

		bool begins_with(const std::string& input, const std::string& find) noexcept
		{
			return (input.rfind(find, 0) == 0);
		}
	} // namespace strutils
} // namespace galaxy