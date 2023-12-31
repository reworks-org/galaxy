///
/// Prefabs.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/ZLib.hpp"

#include "Prefabs.hpp"

namespace galaxy
{
	namespace resource
	{
		std::shared_ptr<core::Prefab> PrefabLoader::operator()(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto data = fs.read(file);
			if (!data.empty())
			{
				const auto base64       = math::decode_zlib(data);
				const auto decompressed = math::decode_base64(base64);

				return std::make_shared<core::Prefab>(nlohmann::json::parse(decompressed));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to read empty prefab '{0}'.", file);
				return nullptr;
			}
		}
	} // namespace resource
} // namespace galaxy
