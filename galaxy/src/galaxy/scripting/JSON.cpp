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
		nlohmann::json read(const std::string& entry)
		{
			auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto  data = fs.read(entry);

			return nlohmann::json::parse(data);
		}

		nlohmann::json read_raw(const std::string& json)
		{
			return nlohmann::json::parse(json);
		}

		bool write(const std::string& entry, const nlohmann::json& json)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			return fs.write(json.dump(4), entry);
		}
	} // namespace json
} // namespace galaxy
