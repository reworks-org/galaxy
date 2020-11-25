///
/// JSONUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
#include <pulsar/Log.hpp>

#include "JSONUtils.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Script identification namespace.
	///
	namespace json
	{
		nlohmann::json parse_from_disk(std::string_view file)
		{
			nlohmann::json j = {};

			auto path = std::filesystem::path {file};
			std::ifstream input;
			input.open(path.string(), std::ifstream::in);

			if (!input.good())
			{
				PL_LOG(PL_FATAL, "Failed to open: {0}.", path.string());
			}
			else
			{
				// Use JSON stream to deserialize data and parse.
				input >> j;
			}

			input.close();

			return std::move(j);
		}

		nlohmann::json parse_from_mem(std::span<char> memory)
		{
			nlohmann::json j = {};

			if (memory.empty())
			{
				PL_LOG(PL_FATAL, "Passed empty buffer to galaxy::json::parse_from_mem().");
			}
			else
			{
				// This is the string parser.
				j = nlohmann::json::parse(memory);
			}

			return std::move(j);
		}
	} // namespace json
} // namespace galaxy