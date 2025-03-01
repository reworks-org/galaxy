///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		Scene::Scene(const std::string& name)
			: Serializable {}
			, m_name {name}
		{
			/*
			auto& w = core::ServiceLocator<core::Window>::ref();
			m_camera.set_viewport(w.frame_width(), w.frame_height());
			auto& nui = core::ServiceLocator<ui::NuklearUI>::ref();
			m_dispatcher.sink<events::WindowResized>().connect<&Scene::on_window_resized>(this);
			m_dispatcher.sink<events::MousePressed>().connect<&ui::NuklearUI::on_mouse_pressed>(nui);
			m_dispatcher.sink<events::MouseWheel>().connect<&ui::NuklearUI::on_mouse_wheel>(nui);
			m_dispatcher.sink<events::KeyChar>().connect<&ui::NuklearUI::on_key_char>(nui);
			m_dispatcher.sink<events::KeyPress>().connect<&ui::NuklearUI::on_key_press>(nui);
			m_dispatcher.sink<events::ContentScale>().connect<&ui::NuklearUI::on_content_scale>(nui);
			*/
		}

		Scene::~Scene()
		{
			m_systems.clear();
		}

		void Scene::add_system(const std::string& system)
		{
			auto& sf = entt::locator<meta::SystemFactory>::value();
			sf.create_system(system, m_systems);
		}

		void Scene::load()
		{
		}

		void Scene::unload()
		{
			entt::locator<sol::state>::value().collect_garbage();
		}

		void Scene::update(core::Registry& registry)
		{
			// graphics::Renderer::ref().flush();
			// m_registry.update(m_b2world);
			// m_b2world.Step(GALAXY_DT, m_velocity_iterations, m_position_iterations);
			for (auto&& system : m_systems)
			{
				// system->update(registry);
			}
		}

		/*void Scene::update_ui()
		{
			const auto view = m_registry.m_entt.view<components::GUI>(entt::exclude<flags::Disabled>);
			for (auto&& [entity, gui] : view.each())
			{
				if (gui.m_update.valid())
				{
					gui.m_update(gui.m_self);
				}
			}
		}*/

		void Scene::render()
		{
			/*
			graphics::Renderer::ref().begin_post();

			// Scene specific.
			graphics::Renderer::ref().submit_camera(m_camera);
			graphics::Renderer::ref().draw();

			graphics::Renderer::ref().end_post();
			graphics::Renderer::ref().begin_default();
			graphics::Renderer::ref().render_post();
			graphics::Renderer::ref().end_default();

			// Scene specific.
			auto& nui = core::ServiceLocator<ui::NuklearUI>::ref();

			nui.new_frame();
			update_ui();
			nui.render();

			graphics::Renderer::ref().end_default();
			*/
		}

		const std::string& Scene::name() const noexcept
		{
			return m_name;
		}

		/*bool Scene::load_world(const std::string& file)
		{
			if (m_world.load(file))
			{
				m_world.parse();
				return true;
			}

			return false;
		}*/

		nlohmann::json Scene::serialize()
		{
			/*
			json["camera"]      = m_camera.serialize();
			json["physics"] = nlohmann::json::object();
			auto& physics   = json.at("physics");

			auto gravity            = m_b2world.GetGravity();
			physics["gravity"]["x"] = gravity.x;
			physics["gravity"]["y"] = gravity.y;

			physics["allow_sleeping"]        = m_b2world.GetAllowSleeping();
			physics["allow_autoclearforces"] = m_b2world.GetAutoClearForces();
			physics["velocity_iterations"]   = m_velocity_iterations;
			physics["position_iterations"]   = m_position_iterations;
			json["name"]                     = m_name;
			json["ldtk_world"]               = m_world.file();
			json["current_map"]              = m_world.get_active() ? m_world.get_active()->name() : "";
			*/

			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;

			json["systems"] = nlohmann::json::object();
			for (auto i = 0; i < m_systems.size(); i++)
			{
				json["systems"][std::to_string(i)] = m_systems[i]->id();
			}

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");

			const auto& systems = json.at("systems");
			m_systems.reserve(systems.size());
			for (const auto& [index, name] : systems.items())
			{
				add_system(name.get<std::string>());
			}

			/*
			m_camera.deserialize(json.at("camera"));
			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			const auto& physics = json.at("physics");
			const auto& gravity = physics.at("gravity");

			m_b2world.SetGravity({gravity.at("x"), gravity.at("y")});
			m_b2world.SetAllowSleeping(physics.at("allow_sleeping"));
			m_b2world.SetAutoClearForces(physics.at("allow_autoclearforces"));
			m_velocity_iterations = physics.at("velocity_iterations");
			m_position_iterations = physics.at("position_iterations");

			if (load_world(json.at("ldtk_world")))
			{
				m_world.set_active(json.at("current_map"));
			}
			*/
		}
	} // namespace scene
} // namespace galaxy
