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

		std::future<void> Scene::load(std::string* text)
		{
			auto future = std::async(std::launch::async, [text]() {
				// TODO
			});

			return future;
		}

		void Scene::unload()
		{
			m_layer_stack.clear();
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

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_layer_stack.serialize();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			unload();

			m_name = json.at("name");
			m_layer_stack.deserialize(json.at("stack"));
		}
	} // namespace state
} // namespace galaxy