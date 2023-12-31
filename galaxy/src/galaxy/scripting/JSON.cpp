///
/// JSON.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "JSON.hpp"

namespace galaxy
{
	namespace json
	{
		std::optional<nlohmann::json> read(const std::string& entry)
		{
			auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto  data = fs.read(entry);

			nlohmann::json json = nlohmann::json::parse(data);
			return std::make_optional(json);
		}

		std::optional<nlohmann::json> read_raw(const std::string& json)
		{
			nlohmann::json parsed = nlohmann::json::parse(json);
			return std::make_optional(parsed);
		}

		bool write(const std::string& entry, const nlohmann::json& json)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			return fs.write(json.dump(4), entry);
		}
	} // namespace json
} // namespace galaxy
