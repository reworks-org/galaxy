///
/// JSONUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "JSONUtils.hpp"

namespace galaxy
{
	namespace json
	{
		std::optional<nlohmann::json> parse_from_disk(std::string_view file)
		{
			const auto path = SL_HANDLE.vfs()->absolute(file);
			if (path != std::nullopt)
			{
				nlohmann::json json;
				std::ifstream input;

				input.open(path.value(), std::ifstream::in);
				if (!input.good())
				{
					input.close();
					GALAXY_LOG(GALAXY_ERROR, "Failed to open: {0}.", path.value());

					return std::nullopt;
				}
				else
				{
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
			if (memory.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Passed empty buffer to json::parse_from_mem().");
				return std::nullopt;
			}
			else
			{
				nlohmann::json json = nlohmann::json::parse(memory);
				return std::make_optional(json);
			}
		}

		const bool save_to_disk(std::string_view path, const nlohmann::json& json)
		{
			return SL_HANDLE.vfs()->save(json.dump(4), path);
		}
	} // namespace json
} // namespace galaxy