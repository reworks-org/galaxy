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
		nlohmann::json parse_from_disk(std::string_view file)
		{
			const auto path = SL_HANDLE.vfs()->absolute(file);

			nlohmann::json json;
			std::ifstream input;

			input.open(path, std::ifstream::in);
			if (!input.good())
			{
				input.close();
				GALAXY_LOG(GALAXY_FATAL, "Failed to open: {0}.", path);
			}
			else
			{
				input >> json;
				input.close();
			}

			return json;
		}

		nlohmann::json parse_from_mem(std::span<char> memory)
		{
			nlohmann::json json;

			if (memory.empty())
			{
				GALAXY_LOG(GALAXY_FATAL, "Passed empty buffer to json::parse_from_mem().");
			}
			else
			{
				json = nlohmann::json::parse(memory);
			}

			return json;
		}

		void save_to_disk(std::string_view path, const nlohmann::json& json)
		{
			SL_HANDLE.vfs()->save(json.dump(4), path);
		}
	} // namespace json
} // namespace galaxy