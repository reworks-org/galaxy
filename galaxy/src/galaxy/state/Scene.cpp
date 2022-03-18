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
			m_shaders.deserialize(m_deserialization_json["resource_shaders"]);
			m_sounds.deserialize(m_deserialization_json["resource_sounds"]);
			m_music.deserialize(m_deserialization_json["resource_music"]);
		}

		void Scene::unload()
		{
			m_layer_stack.clear();
			m_shaders.clear();
		}

		void Scene::events()
		{
			m_layer_stack.events();
		}

		void Scene::update()
		{
			m_layer_stack.update();
		}

		void Scene::pre_render()
		{
			m_layer_stack.pre_render();
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

		ShaderCache& Scene::shader_cache() noexcept
		{
			return m_shaders;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_layer_stack.serialize();

			json["resource_shaders"] = m_shaders.serialize();
			json["resource_sounds"]  = m_sounds.serialize();
			json["resource_music"]   = m_music.serialize();

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
