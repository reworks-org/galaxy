///
/// Serializer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Serializer.hpp"

namespace galaxy
{
	namespace fs
	{
		void Serializer::serialize(core::Scene* scene, std::string_view path)
		{
			nlohmann::json json = "{}"_json;
			json["camera"]      = scene->m_camera.serialize();
			json["world"]       = scene->m_world.serialize();

			const auto file = static_cast<std::string>(path) + scene->get_name() + ".json";
			if (!json::save_to_disk(file, json))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save json to disk using file: {0}.", file);
			}
		}

		void Serializer::deserialize(core::Scene* scene, std::string_view path)
		{
			const auto file     = static_cast<std::string>(path) + scene->get_name() + ".json";
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to deserialize because: load/parse json from disk failed for file: {0}.", file);
			}
			else
			{
				auto& json             = json_opt.value();
				const auto camera_json = json.at("camera");
				const auto world_json  = json.at("world");

				scene->m_camera.deserialize(camera_json);
				scene->m_world.deserialize(world_json);
			}
		}

	} // namespace fs
} // namespace galaxy