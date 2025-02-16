///
/// JSON.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/fs/VirtualFileSystem.hpp"

#include "JSON.hpp"

namespace galaxy
{
	namespace json
	{
		nlohmann::json read(const std::string& entry)
		{
			auto& fs   = entt::locator<fs::VirtualFileSystem>::value();
			auto  data = fs.read(entry);

			return nlohmann::json::parse(data);
		}

		nlohmann::json read_raw(const std::string& json)
		{
			return nlohmann::json::parse(json);
		}

		bool write(const std::string& entry, const nlohmann::json& json)
		{
			auto& fs = entt::locator<fs::VirtualFileSystem>::value();
			return fs.write(json.dump(4), entry);
		}
	} // namespace json
} // namespace galaxy
