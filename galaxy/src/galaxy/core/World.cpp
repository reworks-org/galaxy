///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Map.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/flags/AllowSerialize.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/physics/Constants.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/scripting/JSON.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/resource/Prefabs.hpp"

#include "World.hpp"

namespace galaxy
{
	namespace core
	{
		World::World(scene::Scene* scene)
			: Serializable {}
			, m_b2world {nullptr}
			, m_rendersystem_index {-1}
			, m_scene {scene}
		{
			m_b2world = std::make_unique<b2World>(physics::Constants::gravity);

			register_component<components::Animated>("Animated");
			register_component<components::DrawShader>("DrawShader");
			register_component<components::Flag>("Flag");
			register_component<components::Primitive>("Primitive");
			register_component<components::RigidBody>("RigidBody");
			register_component<components::Script>("Script");
			register_component<components::Sprite>("Sprite");
			register_component<components::Tag>("Tag");
			register_component<components::Text>("Text");
			register_component<components::Transform>("Transform");

			m_registry.on_construct<components::Script>().connect<&World::construct_script>(this);
			m_registry.on_destroy<components::Script>().connect<&World::destruct_script>(this);
			m_registry.on_construct<components::RigidBody>().connect<&World::construct_rigidbody>(this);
			m_registry.on_destroy<components::RigidBody>().connect<&World::destroy_rigidbody>(this);

			// Handle validation.
			register_dependencies<components::Sprite, components::Transform, components::DrawShader>();
			register_dependencies<components::Animated, components::Sprite>();
			register_dependencies<components::RigidBody, components::Transform>();

			// Handle incompatible components.
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Primitive>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Text>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Map>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Animated>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Primitive>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Map>>();
			m_registry.on_construct<components::Primitive>().connect<&entt::registry::remove<components::Animated>>();
			m_registry.on_construct<components::Primitive>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Primitive>().connect<&entt::registry::remove<components::Text>>();
			m_registry.on_construct<components::Primitive>().connect<&entt::registry::remove<components::Map>>();
			m_registry.on_construct<components::Map>().connect<&entt::registry::remove<components::Animated>>();
			m_registry.on_construct<components::Map>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Map>().connect<&entt::registry::remove<components::Text>>();
			m_registry.on_construct<components::Map>().connect<&entt::registry::remove<components::Primitive>>();
		}

		World::~World()
		{
			clear();

			if (m_b2world)
			{
				m_b2world.reset();
				m_b2world = nullptr;
			}

			m_scene = nullptr;
		}

		entt::entity World::create()
		{
			const auto entity = m_registry.create();

			if (!m_registry.all_of<components::Flag>(entity))
			{
				auto& flag = m_registry.emplace<components::Flag>(entity);
				flag.set_flag<flags::AllowSerialize>();
				flag.unset_flag<flags::Enabled>();
			}

			if (!m_registry.all_of<components::Tag>(entity))
			{
				auto& tag = m_registry.emplace<components::Tag>(entity);
				tag.m_tag = "Untagged";
			}

			return entity;
		}

		entt::entity World::create_from_prefab(const std::string& name)
		{
			auto& prefabs = core::ServiceLocator<resource::Prefabs>::ref();
			if (prefabs.has(name))
			{
				return create_from_json(prefabs.get(name)->m_data);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to load missing prefab '{0}'.", name);
				return entt::null;
			}
		}

		entt::entity World::create_from_json(const nlohmann::json& json)
		{
			const auto entity      = m_registry.create();
			const auto& components = json.at("components");

			// Loop over components
			if (!components.empty())
			{
				for (const auto& [key, value] : components.items())
				{
					// Use the assign function to create components for entities without having to know the type.
					m_component_factory[key](entity, value);
				}
			}

			if (!m_registry.all_of<components::Flag>(entity))
			{
				auto& flag = m_registry.emplace<components::Flag>(entity);
				flag.set_flag<flags::AllowSerialize>();
				flag.unset_flag<flags::Enabled>();
			}

			if (!m_registry.all_of<components::Tag>(entity))
			{
				auto& tag = m_registry.emplace<components::Tag>(entity);
				tag.m_tag = "Untagged";
			}

			return entity;
		}

		bool World::is_valid(const entt::entity entity)
		{
			for (const auto& index : m_validations_to_run)
			{
				if (!m_validations[index](entity))
				{
					return false;
				}
			}

			return true;
		}

		void World::update()
		{
			for (const auto& [rigidbody, transform] : m_bodies_to_construct)
			{
				b2BodyDef def {};
				def.position.x    = transform->get_pos().x / physics::Constants::pixels_per_meter;
				def.position.y    = transform->get_pos().y / physics::Constants::pixels_per_meter;
				def.angle         = glm::radians(transform->get_rotation());
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
			m_b2world->Step(GALAXY_DT, physics::Constants::velocity_iterations, physics::Constants::position_iterations);

			for (auto i = 0; i < m_systems.size(); i++)
			{
				m_systems[i]->update(m_scene);
			}
		}

		void World::update_rendersystem()
		{
			if (m_rendersystem_index < m_systems.size() && m_rendersystem_index != -1)
			{
				m_systems[m_rendersystem_index]->update(m_scene);
			}
		}

		void World::clear()
		{
			m_systems.clear();
			m_component_factory.clear();
			m_registry.clear();
			m_bodies_to_construct.clear();

			m_rendersystem_index = -1;
		}

		nlohmann::json World::serialize_entity(const entt::entity entity)
		{
			nlohmann::json json = nlohmann::json::object();
			json["components"]  = nlohmann::json::object();

			auto animated = m_registry.try_get<components::Animated>(entity);
			if (animated != nullptr)
			{
				json["components"]["Animated"] = animated->serialize();
			}

			auto draw_shader = m_registry.try_get<components::DrawShader>(entity);
			if (draw_shader != nullptr)
			{
				json["components"]["DrawShader"] = draw_shader->serialize();
			}

			auto flag = m_registry.try_get<components::Flag>(entity);
			if (flag != nullptr)
			{
				json["components"]["Flag"] = flag->serialize();
			}

			auto primitive = m_registry.try_get<components::Primitive>(entity);
			if (primitive != nullptr)
			{
				json["components"]["Primitive"] = primitive->serialize();
			}

			auto rigidbody = m_registry.try_get<components::RigidBody>(entity);
			if (rigidbody != nullptr)
			{
				json["components"]["RigidBody"] = rigidbody->serialize();
			}

			auto script = m_registry.try_get<components::Script>(entity);
			if (script != nullptr)
			{
				json["components"]["Script"] = script->serialize();
			}

			auto sprite = m_registry.try_get<components::Sprite>(entity);
			if (sprite != nullptr)
			{
				json["components"]["Sprite"] = sprite->serialize();
			}

			auto tag = m_registry.try_get<components::Tag>(entity);
			if (tag != nullptr)
			{
				json["components"]["Tag"] = tag->serialize();
			}

			auto text = m_registry.try_get<components::Text>(entity);
			if (text != nullptr)
			{
				json["components"]["Text"] = text->serialize();
			}

			auto transform = m_registry.try_get<components::Transform>(entity);
			if (transform != nullptr)
			{
				json["components"]["Transform"] = transform->serialize();
			}

			return json;
		}

		nlohmann::json World::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["entities"] = nlohmann::json::array();

			m_registry.each([&](const entt::entity entity) -> void {
				auto flag = m_registry.try_get<components::Flag>(entity);
				if (flag != nullptr)
				{
					if (flag->is_flag_set<flags::AllowSerialize>())
					{
						json["entities"].push_back(serialize_entity(entity));
					}
				}
			});

			return json;
		}

		void World::deserialize(const nlohmann::json& json)
		{
			m_registry.clear();

			const auto& entity_json = json.at("entities");
			for (const auto& entity : entity_json)
			{
				create_from_json(entity);
			}
		}

		void World::construct_script(entt::registry& registry, entt::entity entity)
		{
			auto& script = registry.get<components::Script>(entity);
			if (script.m_self.valid())
			{
				script.m_update = script.m_self["update"];

				if (!script.m_update.valid())
				{
					GALAXY_LOG(GALAXY_ERROR, "Update function not present in '{0}'.", script.file());
				}

				script.m_self["owner"]      = std::ref(registry);
				script.m_self["dispatcher"] = std::ref(m_scene->m_dispatcher);
				script.m_self["id"]         = sol::readonly_property([entity] {
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

		void World::destruct_script(entt::registry& registry, entt::entity entity)
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

		void World::construct_rigidbody(entt::registry& registry, entt::entity entity)
		{
			const auto rigidbody = registry.try_get<components::RigidBody>(entity);
			const auto transform = registry.try_get<components::Transform>(entity);
			if (rigidbody && transform)
			{
				m_bodies_to_construct.emplace_back(rigidbody, transform);
			}
		}

		void World::destroy_rigidbody(entt::registry& registry, entt::entity entity)
		{
			const auto rigidbody = registry.try_get<components::RigidBody>(entity);
			if (rigidbody)
			{
				if (rigidbody->m_body != nullptr)
				{
					m_b2world->DestroyBody(rigidbody->m_body);
					rigidbody->m_body = nullptr;
				}
			}
		}
	} // namespace core
} // namespace galaxy