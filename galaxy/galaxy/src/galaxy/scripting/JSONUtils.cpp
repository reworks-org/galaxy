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

		void save_to_disk(std::string_view path, const nlohmann::json& json)
		{
			auto fp = std::filesystem::path {path};
			std::ofstream out;
			out.open(fp, std::ios::out | std::ios::trunc);
			if (out.good())
			{
				out << json.dump(4);
			}
			else
			{
				PL_LOG(PL_FATAL, "Failed to save json to disk: {0}.", path);
			}

			out.close();
		}
	} // namespace json
} // namespace galaxy