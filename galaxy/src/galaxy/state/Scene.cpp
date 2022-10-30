///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"

#include "Scene.hpp"

namespace galaxy
{
	namespace state
	{
		Scene::Scene() noexcept
			: m_name {"Untitled"}
			, m_layer_stack {this}
			, m_camera {false}
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
			m_camera.process_events();
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
			m_layer_stack.update_rendersystem();
		}

		void Scene::set_name(const std::string& name) noexcept
		{
			m_name = name;
		}

		const std::string& Scene::get_name() const noexcept
		{
			return m_name;
		}

		Layers& Scene::layers() noexcept
		{
			return m_layer_stack;
		}

		graphics::Camera& Scene::get_camera() noexcept
		{
			return m_camera;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["stack"]       = m_layer_stack.serialize();
			json["camera"]      = m_camera.serialize();

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			unload();

			m_name = json.at("name");
			m_layer_stack.deserialize(json.at("stack"));
			m_camera.deserialize(json.at("camera"));
		}
	} // namespace state
} // namespace galaxy