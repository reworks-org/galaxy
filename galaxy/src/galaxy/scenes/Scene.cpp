///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>

#include "Scene.hpp"

namespace galaxy
{
	namespace scenes
	{
		Scene::Scene(std::string_view name, Types type) noexcept
		    : Serializable {this}, m_type {type}, m_name {name}
		{
		}

		const std::string& Scene::get_name() const noexcept
		{
			return m_name;
		}

		const Types Scene::get_type() const noexcept
		{
			return m_type;
		}

		core::World& Scene::world() noexcept
		{
			return m_world;
		}

		graphics::Camera& Scene::camera() noexcept
		{
			return m_camera;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["name"]   = m_name;
			json["type"]   = static_cast<std::string>(magic_enum::enum_name<Types>(m_type));
			json["camera"] = m_camera.serialize();
			json["world"]  = m_world.serialize();
			json["sub"]    = sub_serialize();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
			m_type = magic_enum::enum_cast<Types>(json.at("type").get<std::string>()).value();

			const auto camera_json = json.at("camera");
			const auto world_json  = json.at("world");

			m_camera.deserialize(camera_json);
			m_world.deserialize(world_json);

			sub_deserialize(json.at("sub"));
		}
	} // namespace scenes
} // namespace galaxy