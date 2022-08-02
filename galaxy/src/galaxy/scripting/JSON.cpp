///
/// JSONUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "JSON.hpp"

namespace galaxy
{
	namespace json
	{
		std::optional<nlohmann::json> parse_from_disk(std::string_view file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto path = fs.find(file);
			if (path.m_code == fs::FileCode::FOUND)
			{
				std::ifstream input {path.m_string, std::ifstream::in};

				if (!input.good())
				{
					input.close();
					GALAXY_LOG(GALAXY_ERROR, "Failed to read: {0}.", path.m_string);

					return std::nullopt;
				}
				else
				{
					nlohmann::json json;

					input >> json;
					input.close();

					return std::make_optional(json);
				}
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<nlohmann::json> parse_from_mem(std::span<char> memory)
		{
			if (!memory.empty())
			{
				nlohmann::json json = nlohmann::json::parse(memory);
				return std::make_optional(json);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to parse empty memory buffer for json data.");
				return std::nullopt;
			}
		}

		bool save_to_disk(std::string_view path, const nlohmann::json& json)
		{
			return core::ServiceLocator<fs::VirtualFileSystem>::ref().save(json.dump(4), path);
		}
	} // namespace json
} // namespace galaxy