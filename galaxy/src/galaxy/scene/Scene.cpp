///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <BS_thread_pool.hpp>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/Loader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/scripting/JSON.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/systems/AnimationSystem.hpp"
#include "galaxy/systems/PhysicsSystem.hpp"
#include "galaxy/systems/ScriptSystem.hpp"
#include "galaxy/ui/NuklearUI.hpp"

#include "Scene.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26487)
#endif

namespace galaxy
{
	namespace scene
	{
		Scene::Scene()
			: Serializable {}
			, m_enabled {false}
			, m_name {"Untitled"}
			, m_camera {}
			, m_rendersystem_index {0}
			, m_b2world {nullptr}
			, m_velocity_iterations {8}
			, m_position_iterations {3}
		{
			init();
		}

		Scene::Scene(const std::string& name)
			: Serializable {}
			, m_enabled {false}
			, m_name {"Untitled"}
			, m_camera {}
			, m_rendersystem_index {0}
			, m_b2world {nullptr}
			, m_velocity_iterations {8}
			, m_position_iterations {3}
		{
			init();
		}

		Scene::~Scene()
		{
		}

		void Scene::init()
		{
			m_b2world = std::make_unique<b2World>(b2Vec2 {0.0f, 0.0f});

			// Handle on_* events.
			m_registry.on_construct<components::RigidBody>().connect<&Scene::construct_rigidbody>(this);
			m_registry.on_destroy<components::RigidBody>().connect<&Scene::destroy_rigidbody>(this);
			m_registry.on_construct<components::Script>().connect<&Scene::construct_script>(this);
			m_registry.on_destroy<components::Script>().connect<&Scene::destruct_script>(this);
			m_registry.on_construct<components::GUI>().connect<&Scene::construct_nui>(this);
			m_registry.on_destroy<components::GUI>().connect<&Scene::construct_nui>(this);
			m_registry.on_construct<flags::Disabled>().connect<&Scene::disable_entity>(this);
			m_registry.on_destroy<flags::Disabled>().connect<&Scene::enable_entity>(this);

			// Handle incompatible components.
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Circle>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Ellipse>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Point>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Polygon>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Polyline>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Text>>();

			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Circle>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Ellipse>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Point>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Polygon>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Polyline>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Sprite>>();

			m_registry.on_construct<components::Circle>().connect<&entt::registry::remove<components::Ellipse>>();
			m_registry.on_construct<components::Circle>().connect<&entt::registry::remove<components::Point>>();
			m_registry.on_construct<components::Circle>().connect<&entt::registry::remove<components::Polygon>>();
			m_registry.on_construct<components::Circle>().connect<&entt::registry::remove<components::Polyline>>();
			m_registry.on_construct<components::Circle>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Circle>().connect<&entt::registry::remove<components::Text>>();

			m_registry.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Circle>>();
			m_registry.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Point>>();
			m_registry.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Polygon>>();
			m_registry.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Polyline>>();
			m_registry.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Text>>();

			m_registry.on_construct<components::Point>().connect<&entt::registry::remove<components::Circle>>();
			m_registry.on_construct<components::Point>().connect<&entt::registry::remove<components::Ellipse>>();
			m_registry.on_construct<components::Point>().connect<&entt::registry::remove<components::Polygon>>();
			m_registry.on_construct<components::Point>().connect<&entt::registry::remove<components::Polyline>>();
			m_registry.on_construct<components::Point>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Point>().connect<&entt::registry::remove<components::Text>>();

			m_registry.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Circle>>();
			m_registry.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Ellipse>>();
			m_registry.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Point>>();
			m_registry.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Polyline>>();
			m_registry.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Text>>();

			m_registry.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Circle>>();
			m_registry.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Ellipse>>();
			m_registry.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Point>>();
			m_registry.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Polygon>>();
			m_registry.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Text>>();

			// Handle rendercommand creation.
			m_registry.on_construct<components::Sprite>().connect<&Scene::construct_rendercommand<components::Sprite>>(this);
			m_registry.on_construct<components::Text>().connect<&Scene::construct_rendercommand<components::Text>>(this);
			m_registry.on_construct<components::Circle>().connect<&Scene::construct_rendercommand<components::Circle>>(this);
			m_registry.on_construct<components::Ellipse>().connect<&Scene::construct_rendercommand<components::Ellipse>>(this);
			m_registry.on_construct<components::Point>().connect<&Scene::construct_rendercommand<components::Point>>(this);
			m_registry.on_construct<components::Polygon>().connect<&Scene::construct_rendercommand<components::Polygon>>(this);
			m_registry.on_construct<components::Polyline>().connect<&Scene::construct_rendercommand<components::Polyline>>(this);

			create_system<systems::ScriptSystem>();
			create_system<systems::PhysicsSystem>();
			create_system<systems::AnimationSystem>();
			create_system<systems::RenderSystem>();

			auto& dispatcher = core::ServiceLocator<entt::dispatcher>::ref();
			dispatcher.sink<events::WindowResized>().connect<&Scene::on_window_resized>(this);
		}

		void Scene::load()
		{
			// load_maps();
		}

		void Scene::unload()
		{
			// core::ServiceLocator<sol::state>::ref().collect_garbage();
			core::ServiceLocator<entt::dispatcher>::ref().disconnect(m_camera);
		}

		void Scene::update()
		{
			for (const auto& [rigidbody, transform] : m_bodies_to_construct)
			{
				b2BodyDef def {};
				def.position.x    = transform->m_tf.get_pos().x / GALAXY_WORLD_TO_BOX;
				def.position.y    = transform->m_tf.get_pos().y / GALAXY_WORLD_TO_BOX;
				def.angle         = glm::radians(transform->m_tf.get_rotation());
				def.type          = rigidbody->m_type;
				def.fixedRotation = rigidbody->m_fixed_rotation;
				def.bullet        = rigidbody->m_bullet;

				rigidbody->m_body = m_b2world->CreateBody(&def);

				b2PolygonShape shape;
				shape.SetAsBox(rigidbody->m_shape.x, rigidbody->m_shape.y);

				b2FixtureDef fixture;
				fixture.shape                = &shape;
				fixture.density              = rigidbody->m_density;
				fixture.friction             = rigidbody->m_friction;
				fixture.restitution          = rigidbody->m_restitution;
				fixture.restitutionThreshold = rigidbody->m_restitution_threshold;

				rigidbody->m_body->CreateFixture(&fixture);
			}

			m_bodies_to_construct.clear();
			m_b2world->Step(GALAXY_DT, m_velocity_iterations, m_position_iterations);

			for (auto&& system : m_systems)
			{
				system->update(this);
			}
		}

		void Scene::render()
		{
			auto& nui = core::ServiceLocator<ui::NuklearUI>::ref();

			graphics::Renderer::ref().submit_camera(m_camera);
			graphics::Renderer::ref().draw();
			graphics::Renderer::ref().flush();

			nui.new_frame();
			update_ui();
			// Dont need to flush nui since it flushes itself.
			nui.render();
		}

		void Scene::on_window_resized(const events::WindowResized& e)
		{
			m_camera.set_viewport(static_cast<float>(e.width), static_cast<float>(e.height));
		}

		void Scene::update_ui()
		{
			const auto view = m_registry.view<components::GUI>(entt::exclude<flags::Disabled>);
			for (auto&& [entity, script] : view.each())
			{
				if (script.m_update.valid())
				{
					script.m_update(script.m_self);
				}
			}
		}

		void Scene::update_rendering()
		{
			if (m_rendersystem_index >= 0 && m_rendersystem_index < m_systems.size())
			{
				m_systems[m_rendersystem_index]->update(this);
			}
		}

		entt::entity Scene::create()
		{
			const auto entity = m_registry.create();

			m_registry.emplace<flags::Disabled>(entity);

			if (!m_registry.all_of<components::Tag>(entity))
			{
				auto& tag = m_registry.emplace<components::Tag>(entity);
				tag.m_tag = "Untagged";
			}

			return entity;
		}

		entt::entity Scene::create_from_prefab(const std::string& name)
		{
			auto& prefabs = core::ServiceLocator<resource::Prefabs>::ref();
			if (prefabs.has(name))
			{
				return prefabs.get(name)->to_entity(m_registry);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to load missing prefab '{0}'.", name);
				return entt::null;
			}
		}

		bool Scene::is_valid(const entt::entity entity)
		{
			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			for (const auto& hash : em.get_validation_list())
			{
				if (!(em.get_validations().at(hash)(entity, m_registry)))
				{
					return false;
				}
			}

			return true;
		}

		/*
		void Scene::load_maps()
		{
			core::ServiceLocator<core::Loader>::ref().load_maps(m_assigned_maps, m_registry);
		}
		*/

		/*
		void Scene::set_active_map(const std::string& map)
		{
			if (m_map)
			{
				m_map->disable();
			}

			m_map = !map.empty() ? core::ServiceLocator<resource::Maps>::ref().get(map) : nullptr;

			if (m_map)
			{
				m_map->enable();
			}
		}
		*/

		void Scene::enable()
		{
			m_enabled = true;
			load();
		}

		void Scene::disable()
		{
			m_enabled = false;
			unload();
		}

		bool Scene::enabled() const
		{
			return m_enabled;
		}

		nlohmann::json Scene::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["enabled"]     = m_enabled;
			json["camera"]      = m_camera.serialize();
			json["entities"]    = nlohmann::json::array();

			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			for (const auto& [entity] : m_registry.view<entt::entity>(entt::exclude<flags::DenySerialization>).each())
			{
				json["entities"].push_back(em.serialize_entity(entity, m_registry));
			}

			json["physics"] = nlohmann::json::object();
			auto& physics   = json.at("physics");

			auto gravity            = m_b2world->GetGravity();
			physics["gravity"]["x"] = gravity.x;
			physics["gravity"]["y"] = gravity.y;

			physics["allow_sleeping"]        = m_b2world->GetAllowSleeping();
			physics["allow_autoclearforces"] = m_b2world->GetAutoClearForces();
			physics["velocity_iterations"]   = m_velocity_iterations;
			physics["position_iterations"]   = m_position_iterations;
			json["name"]                     = m_name;
			// json["maps"]                     = m_assigned_maps;
			//  json["active_map"]               = m_map ? m_map->get_name() : "";

			return json;
		}

		void Scene::deserialize(const nlohmann::json& json)
		{
			m_enabled = json.at("enabled");
			m_camera.deserialize(json.at("camera"));
			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			m_bodies_to_construct.clear();
			m_registry.clear();

			const auto& physics = json.at("physics");
			const auto& gravity = physics.at("gravity");

			m_b2world->SetGravity({gravity.at("x"), gravity.at("y")});
			m_b2world->SetAllowSleeping(physics.at("allow_sleeping"));
			m_b2world->SetAutoClearForces(physics.at("allow_autoclearforces"));
			m_velocity_iterations = physics.at("velocity_iterations");
			m_position_iterations = physics.at("position_iterations");

			const auto& entity_json = json.at("entities");
			for (const auto& data : entity_json)
			{
				const auto entity = em.deserialize_entity(data, m_registry);

				m_registry.emplace<flags::Disabled>(entity);

				if (!m_registry.all_of<components::Tag>(entity))
				{
					auto& tag = m_registry.emplace<components::Tag>(entity);
					tag.m_tag = "Untagged";
				}
			}

			m_name = json.at("name");

			// m_assigned_maps = json.at("maps");
			// set_active_map(json.at("active_map"));
		}

		void Scene::construct_rigidbody(entt::registry& registry, entt::entity entity)
		{
			auto transform = registry.try_get<components::Transform>(entity);
			if (!transform)
			{
				transform = &registry.emplace<components::Transform>(entity);
			}

			m_bodies_to_construct.emplace_back(&registry.get<components::RigidBody>(entity), transform);
		}

		void Scene::destroy_rigidbody(entt::registry& registry, entt::entity entity) const
		{
			auto& rigidbody = registry.get<components::RigidBody>(entity);

			if (rigidbody.m_body != nullptr)
			{
				m_b2world->DestroyBody(rigidbody.m_body);
				rigidbody.m_body = nullptr;
			}
		}

		void Scene::construct_script(entt::registry& registry, entt::entity entity)
		{
			auto& script = registry.get<components::Script>(entity);
			auto& state  = core::ServiceLocator<sol::state>::ref();

			auto result = state.load_file(script.file());
			if (result.valid())
			{
				script.m_self = result.call();

				if (script.m_self.valid())
				{
					script.m_update = script.m_self["update"];

					if (!script.m_update.valid())
					{
						GALAXY_LOG(GALAXY_ERROR, "Update function not present in '{0}'.", script.file());
					}

					script.m_self["owner"] = std::ref(registry);
					script.m_self["id"]    = sol::readonly_property([entity] {
                        return entity;
                    });

					sol::function init = script.m_self["construct"];
					if (init.valid())
					{
						init(script.m_self);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to validate script '{0}'. Make sure its in the correct format.", script.file());
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load script '{0}' because '{1}'.", script.file(), magic_enum::enum_name(result.status()));
			}
		}

		void Scene::destruct_script(entt::registry& registry, entt::entity entity)
		{
			auto& script = registry.get<components::Script>(entity);

			if (script.m_self.valid())
			{
				sol::function destruct = script.m_self["destruct"];
				if (destruct.valid())
				{
					destruct(script.m_self);
				}

				script.m_self.abandon();
			}
		}

		void Scene::construct_nui(entt::registry& registry, entt::entity entity)
		{
			auto& ui    = registry.get<components::GUI>(entity);
			auto& state = core::ServiceLocator<sol::state>::ref();
			auto& nui   = core::ServiceLocator<ui::NuklearUI>::ref();
			auto& fs    = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto script = fs.read(ui.file());
			if (!script.empty())
			{
				auto result = state.load(script);

				if (result.valid())
				{
					ui.m_self = result.call();

					if (ui.m_self.valid())
					{
						ui.m_self["ctx"] = nui.ctx();
						ui.m_update      = ui.m_self["do_ui"];

						if (!ui.m_update.valid())
						{
							GALAXY_LOG(GALAXY_ERROR, "Update function not present in ui script '{0}'.", ui.file());
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to validate ui script '{0}'. Make sure its in the correct format.", ui.file());
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load ui script '{0}' because '{1}'.", ui.file(), magic_enum::enum_name(result.status()));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read script '{0}'.", ui.file());
			}
		}

		void Scene::destruct_nui(entt::registry& registry, entt::entity entity)
		{
			auto& ui = registry.get<components::GUI>(entity);
			if (ui.m_self.valid())
			{
				ui.m_self.abandon();
			}
		}

		void Scene::enable_entity(entt::registry& registry, entt::entity entity)
		{
			auto rb = registry.try_get<components::RigidBody>(entity);
			if (rb)
			{
				rb->m_body->SetEnabled(true);
			}
		}

		void Scene::disable_entity(entt::registry& registry, entt::entity entity)
		{
			auto rb = registry.try_get<components::RigidBody>(entity);
			if (rb)
			{
				rb->m_body->SetEnabled(false);
			}
		}
	} // namespace scene
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
