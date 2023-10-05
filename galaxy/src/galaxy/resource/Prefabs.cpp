///
/// Prefabs.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/ZLib.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Prefabs.hpp"

namespace galaxy
{
	namespace resource
	{
		std::shared_ptr<core::Prefab> PrefabLoader::operator()(const std::string& file)
		{
			auto& fs        = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			const auto data = fs.open(file);
			if (!data.empty())
			{
				const auto name         = std::filesystem::path(file).stem().string();
				const auto base64       = algorithm::decode_zlib(data);
				const auto decompressed = algorithm::decode_base64(base64);

				return std::make_shared<core::Prefab>(nlohmann::json::parse(decompressed));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to load empty prefab {0}.", file);
				return nullptr;
			}
		}
	} // namespace resource
} // namespace galaxy