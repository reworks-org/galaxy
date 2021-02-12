///
/// Serializer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Serializer.hpp"

namespace galaxy
{
	namespace fs
	{
		void Serializer::serialize(core::Scene* scene, std::string_view file)
		{
			nlohmann::json json = "{}"_json;
			json["camera"]      = scene->m_camera.serialize();
			json["world"]       = scene->m_world.serialize();

			json::save_to_disk(file, json);
		}

		void Serializer::deserialize(core::Scene* scene, std::string_view file)
		{
			nlohmann::json json    = json::parse_from_disk(file);
			const auto camera_json = json.at("camera");
			const auto world_json  = json.at("world");

			scene->m_camera.deserialize(camera_json);
			scene->m_world.deserialize(world_json);
		}
	} // namespace fs
} // namespace galaxy