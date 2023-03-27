///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer.hpp"

#include "Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene::Scene()
			: m_camera {false}
			, m_world {this}
			, m_map {this}
			, m_name {"Untitled"}
			, m_layer_stack {this}
		{
		}

		Scene::~Scene()
		{
			unload();
		}

		void Scene::load()
		{
		}

		void Scene::unload()
		{
			// Cleanup loaded lua data from entities.
			auto& lua = core::ServiceLocator<sol::state>::ref();
			lua.collect_garbage();
		}

		void Scene::events()
		{
			m_layer_stack.events();
		}

		void Scene::update()
		{
			m_layer_stack.update();
		}

		void Scene::render()
		{
			graphics::Renderer::buffer_camera(m_camera);
			m_layer_stack.render();
		}

		void Scene::update_rendersystem()
		{
			for (const auto& layer : m_layer_stack.stack())
			{
				if (layer->get_type() == "Runtime")
				{
					m_world.update_rendersystem();
					break;
				}
			}
		}

		void Scene::set_name(const std::string& name)
		{
			m_name = name;
		}

		const std::string& Scene::get_name() const
		{
			return m_name;
		}

		Layers& Scene::layers()
		{
			return m_layer_stack;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_layer_stack.serialize();
			json["camera"]      = m_camera.serialize();
			json["world"]       = m_world.serialize();
			json["active_map"]  = m_map.get_name();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			unload();

			m_name = json.at("name");
			m_layer_stack.deserialize(json.at("stack"));
			m_camera.deserialize(json.at("camera"));
			m_world.deserialize(json.at("world"));
			m_map.load_map(json.at("active_map"));
		}
	} // namespace scene
} // namespace galaxy