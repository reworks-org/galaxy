///
/// FontBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "FontBook.hpp"

namespace galaxy
{
	namespace res
	{
		FontBook::FontBook(std::string_view file)
		{
			create_from_json(file);
		}

		FontBook::~FontBook() noexcept
		{
			clear();
		}

		void FontBook::create_from_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for Fontbook.", file);
			}
			else
			{
				auto& json = json_opt.value();

				for (auto& [name, arr] : json.at("fontbook").items())
				{
					create(name, arr[0].get<std::string>(), arr[1].get<int>());
				}
			}
		}

		void FontBook::clear() noexcept
		{
			m_resources.clear();
		}
	} // namespace res
} // namespace galaxy