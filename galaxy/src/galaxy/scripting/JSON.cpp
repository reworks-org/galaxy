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
		std::optional<nlohmann::json> read_vfs(const std::string& entry)
		{
			auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto  data = fs.read<meta::FSTextR>(entry);

			nlohmann::json json = nlohmann::json::parse(data);
			return std::make_optional(json);
		}

		std::optional<nlohmann::json> read_raw(const std::string& json)
		{
			nlohmann::json parsed = nlohmann::json::parse(json);
			return std::make_optional(parsed);
		}

		std::optional<nlohmann::json> read_disk(const std::string& file)
		{
			auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto  data = fs.read_disk<meta::FSTextR>(file);

			nlohmann::json json = nlohmann::json::parse(data);
			return std::make_optional(json);
		}

		bool write_vfs(const std::string& entry, const nlohmann::json& json)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			return fs.write<meta::FSTextW>(json.dump(4), entry);
		}

		bool write_disk(const std::string& file, const nlohmann::json& json)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			return fs.write_disk<meta::FSTextW>(json.dump(4), file);
		}
	} // namespace json
} // namespace galaxy
