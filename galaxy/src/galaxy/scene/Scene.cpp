///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/Loader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/scripting/JSON.hpp"
#include "galaxy/scripting/Lua.hpp"

#include "Scene.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26487)
#pragma warning(disable : 4244)
#endif

namespace galaxy
{
	namespace scene
	{
		Scene::Scene(const std::string& name)
			: Serializable {}
			, m_name {name}
			, m_camera {}
			, m_b2world {b2Vec2 {0.0f, 0.0f}}
			, m_velocity_iterations {8}
			, m_position_iterations {3}
		{
			auto& w = core::ServiceLocator<core::Window>::ref();
			m_camera.set_viewport(w.frame_width(), w.frame_height());
		}

		Scene::~Scene()
		{
		}

		void Scene::load()
		{
			core::ServiceLocator<core::Window>::ref().set_dispatcher(&m_dispatcher);
		}

		void Scene::unload()
		{
			core::ServiceLocator<sol::state>::ref().collect_garbage();
		}

		void Scene::update()
		{
			graphics::Renderer::ref().flush();
			m_registry.update(m_b2world);
			m_b2world.Step(GALAXY_DT, m_velocity_iterations, m_position_iterations);
		}

		void Scene::render()
		{
			graphics::Renderer::ref().begin_post();

			// Scene specific.
			graphics::Renderer::ref().submit_camera(m_camera);
			graphics::Renderer::ref().draw();

			graphics::Renderer::ref().end_post();
			graphics::Renderer::ref().begin_default();
			graphics::Renderer::ref().render_post();
			graphics::Renderer::ref().end_default();
		}

		bool Scene::load_world(const std::string& file)
		{
			if (m_world.load(file))
			{
				m_world.parse();
				return true;
			}

			return false;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["camera"]      = m_camera.serialize();
			json["entities"]    = nlohmann::json::array();

			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			for (const auto& [entity] : m_registry.m_entt.view<entt::entity>(entt::exclude<flags::DenySerialization>).each())
			{
				json["entities"].push_back(em.serialize_entity(entity, m_registry.m_entt));
			}

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

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_camera.deserialize(json.at("camera"));
			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			m_registry.clear();

			const auto& physics = json.at("physics");
			const auto& gravity = physics.at("gravity");

			m_b2world.SetGravity({gravity.at("x"), gravity.at("y")});
			m_b2world.SetAllowSleeping(physics.at("allow_sleeping"));
			m_b2world.SetAutoClearForces(physics.at("allow_autoclearforces"));
			m_velocity_iterations = physics.at("velocity_iterations");
			m_position_iterations = physics.at("position_iterations");

			const auto& entity_json = json.at("entities");
			for (const auto& data : entity_json)
			{
				const auto entity = em.deserialize_entity(data, m_registry.m_entt);

				if (!m_registry.m_entt.all_of<components::Tag>(entity))
				{
					auto& tag = m_registry.m_entt.emplace<components::Tag>(entity);
					tag.m_tag = "Untagged";
				}
			}

			m_name = json.at("name");

			if (load_world(json.at("ldtk_world")))
			{
				m_world.set_active(json.at("current_map"));
			}
		}

	} // namespace scene
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
