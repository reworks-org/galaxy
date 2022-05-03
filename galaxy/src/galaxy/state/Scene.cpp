///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Scene.hpp"

namespace galaxy
{
	namespace state
	{
		Scene::Scene() noexcept
			: m_name {"New Scene"}
		{
		}

		Scene::~Scene()
		{
			unload();
		}

		void Scene::load()
		{
			// Need to only load shaders for this scene.
			m_resources.m_shaders.deserialize(m_deserialization_json["resource_shaders"]);
			m_resources.m_sounds.deserialize(m_deserialization_json["resource_sounds"]);
			m_resources.m_music.deserialize(m_deserialization_json["resource_music"]);
			m_resources.m_atlas.deserialize(m_deserialization_json["resource_textures"]);

			m_layer_stack.set_resources(&m_resources);
		}

		void Scene::unload()
		{
			m_layer_stack.clear();
			m_resources.m_shaders.clear();
			m_resources.m_sounds.clear();
			m_resources.m_music.clear();
			m_resources.m_atlas.clear();
		}

		void Scene::update()
		{
			m_layer_stack.update();
		}

		void Scene::render()
		{
			m_layer_stack.render();
		}

		void Scene::set_name(const std::string& name) noexcept
		{
			m_name = name;
		}

		const std::string& Scene::get_name() const noexcept
		{
			return m_name;
		}

		LayerStack& Scene::get_stack() noexcept
		{
			return m_layer_stack;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_layer_stack.serialize();

			json["resource_shaders"]  = m_resources.m_shaders.serialize();
			json["resource_sounds"]   = m_resources.m_sounds.serialize();
			json["resource_music"]    = m_resources.m_music.serialize();
			json["resource_textures"] = m_resources.m_atlas.serialize();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			unload();

			m_name = json.at("name");
			m_layer_stack.deserialize(json.at("stack"));

			// We dont load resources here, we backup the json and load later on call.
			m_deserialization_json = json;
		}
	} // namespace state
} // namespace galaxy