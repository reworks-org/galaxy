///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <sol/sol.hpp>

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/UIScript.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/AllowSerialize.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/scripting/JSON.hpp"
#include "galaxy/scripting/Lua.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/ui/NuklearUI.hpp"

#include "World.hpp"

namespace galaxy
{
	namespace core
	{
		World::World(scene::Scene* scene)
			: Serializable {}
			, m_b2world {nullptr}
			, m_scene {scene}
			, m_velocity_iterations {8}
			, m_position_iterations {3}
			, m_pixels_per_meter {64.0f}
		{
			m_b2world = std::make_unique<b2World>(b2Vec2 {0.0f, 0.0f});

			// Handle on_* events.
			m_registry.on_construct<components::Script>().connect<&World::construct_script>(this);
			m_registry.on_destroy<components::Script>().connect<&World::destruct_script>(this);
			m_registry.on_construct<components::RigidBody>().connect<&World::construct_rigidbody>(this);
			m_registry.on_destroy<components::RigidBody>().connect<&World::destroy_rigidbody>(this);
			m_registry.on_construct<components::UIScript>().connect<&World::construct_nui>(this);
			m_registry.on_destroy<components::UIScript>().connect<&World::construct_nui>(this);

			// Handle incompatible components.
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Primitive>>();
			m_registry.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Text>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Animated>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Text>().connect<&entt::registry::remove<components::Primitive>>();
			m_registry.on_construct<components::Primitive>().connect<&entt::registry::remove<components::Animated>>();
			m_registry.on_construct<components::Primitive>().connect<&entt::registry::remove<components::Sprite>>();
			m_registry.on_construct<components::Primitive>().connect<&entt::registry::remove<components::Text>>();
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
				return prefabs.get(name)->to_entity(m_registry);
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

			if (!components.empty())
			{
				auto& em = core::ServiceLocator<meta::EntityMeta>::ref();
				for (const auto& [key, value] : components.items())
				{
					// Use the assign function to create components for entities without having to know the type.
					em.json_factory(key, entity, m_registry, value);
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

		void World::update()
		{
			for (const auto& [rigidbody, transform] : m_bodies_to_construct)
			{
				b2BodyDef def {};
				def.position.x    = transform->get_pos().x / m_pixels_per_meter;
				def.position.y    = transform->get_pos().y / m_pixels_per_meter;
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
			m_b2world->Step(GALAXY_DT, m_velocity_iterations, m_position_iterations);

			for (auto i = 0; i < m_systems.size(); i++)
			{
				m_systems[i]->update(m_scene);
			}
		}

		void World::update_rendersystem()
		{
			// Possible to do this in parallel but overhead would cost more than perf saved.
			auto& renderer = ServiceLocator<graphics::Renderer>::ref();

			const auto spr_group  = m_registry.group<components::Sprite>(entt::get<components::Transform, components::Flag>);
			const auto prim_group = m_registry.group<components::Primitive>(entt::get<components::Transform, components::Flag>);
			const auto text_group = m_registry.group<components::Text>(entt::get<components::Transform, components::Flag>);

			graphics::RenderCommand cmd;

			for (auto&& [entity, sprite, transform, flag] : spr_group.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(sprite.get_width() * 0.5f, sprite.get_height() * 0.5f);

					cmd.instances              = 1;
					cmd.mode                   = graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
					cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
					cmd.uniform_data.colour    = {1.0f, 1.0f, 1.0f, sprite.get_opacity()};
					cmd.uniform_data.transform = transform.get_transform();
					cmd.uniform_data.point     = false;
					cmd.uniform_data.textured  = true;
					cmd.renderable             = &sprite;

					renderer.submit(cmd);
				}
			}

			for (auto&& [entity, primitive, transform, flag] : prim_group.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(primitive.get_width() * 0.5f, primitive.get_height() * 0.5f);

					cmd.instances              = 1;
					cmd.mode                   = primitive.get_mode();
					cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
					cmd.uniform_data.colour    = primitive.m_colour.to_vec4();
					cmd.uniform_data.transform = transform.get_transform();
					cmd.uniform_data.point     = primitive.get_shape() == graphics::Shape::POINT;
					cmd.uniform_data.textured  = false;
					cmd.renderable             = &primitive;

					renderer.submit(cmd);
				}
			}

			for (auto&& [entity, text, transform, flag] : text_group.each())
			{
				if (flag.is_flag_set<flags::Enabled>())
				{
					transform.set_origin(text.get_width() * 0.5f, text.get_height() * 0.5f);

					cmd.instances              = 1;
					cmd.mode                   = graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
					cmd.uniform_data.entity    = static_cast<int>(entt::to_integral(entity));
					cmd.uniform_data.colour    = text.m_colour.to_vec4();
					cmd.uniform_data.transform = transform.get_transform();
					cmd.uniform_data.point     = false;
					cmd.uniform_data.textured  = true;
					cmd.renderable             = &text;

					renderer.submit(cmd);
				}
			}
		}

		bool World::is_valid(const entt::entity entity)
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

		void World::clear()
		{
			m_systems.clear();
			m_registry.clear();
			m_bodies_to_construct.clear();
		}

		b2World* World::b2world()
		{
			return m_b2world.get();
		}

		const scene::Scene* const World::scene() const
		{
			return m_scene;
		}

		nlohmann::json World::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["entities"]    = nlohmann::json::array();

			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			for (auto&& [entity, flag] : m_registry.view<components::Flag>().each())
			{
				if (flag.is_flag_set<flags::AllowSerialize>())
				{
					json["entities"].push_back(em.serialize_entity(entity, m_registry));
				}
			}

			json["physics"] = nlohmann::json::object();
			auto& physics   = json.at("physics");

			auto gravity            = m_b2world->GetGravity();
			physics["gravity"]["x"] = gravity.x;
			physics["gravity"]["y"] = gravity.y;

			physics["allow_sleeping"]        = m_b2world->GetAllowSleeping();
			physics["allow_autoclearforces"] = m_b2world->GetAutoClearForces();
			physics["pixels_per_meter"]      = m_pixels_per_meter;
			physics["velocity_iterations"]   = m_velocity_iterations;
			physics["position_iterations"]   = m_position_iterations;

			return json;
		}

		void World::deserialize(const nlohmann::json& json)
		{
			m_bodies_to_construct.clear();
			m_registry.clear();

			const auto& physics = json.at("physics");
			const auto& gravity = physics.at("gravity");

			m_b2world->SetGravity({gravity.at("x"), gravity.at("y")});
			m_b2world->SetAllowSleeping(physics.at("allow_sleeping"));
			m_b2world->SetAutoClearForces(physics.at("allow_autoclearforces"));
			m_pixels_per_meter    = physics.at("pixels_per_meter");
			m_velocity_iterations = physics.at("velocity_iterations");
			m_position_iterations = physics.at("position_iterations");

			const auto& entity_json = json.at("entities");
			for (const auto& entity : entity_json)
			{
				create_from_json(entity);
			}
		}

		void World::construct_script(entt::registry& registry, entt::entity entity)
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
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load script '{0}' because '{1}'.", script.file(), magic_enum::enum_name(result.status()));
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
			auto transform = registry.try_get<components::Transform>(entity);
			if (!transform)
			{
				transform = &registry.emplace<components::Transform>(entity);
			}

			components::RigidBody* rigidbody = &registry.get<components::RigidBody>(entity);
			m_bodies_to_construct.emplace_back(rigidbody, transform);
		}

		void World::destroy_rigidbody(entt::registry& registry, entt::entity entity)
		{
			auto& rigidbody = registry.get<components::RigidBody>(entity);

			if (rigidbody.m_body != nullptr)
			{
				m_b2world->DestroyBody(rigidbody.m_body);
				rigidbody.m_body = nullptr;
			}
		}

		void World::construct_nui(entt::registry& registry, entt::entity entity)
		{
			auto& ui    = registry.get<components::UIScript>(entity);
			auto& state = core::ServiceLocator<sol::state>::ref();
			auto& nui   = core::ServiceLocator<ui::NuklearUI>::ref();

			auto result = state.load_file(ui.file());
			if (result.valid())
			{
				ui.m_self = result.call();

				if (ui.m_self.valid())
				{
					ui.m_self["ctx"]        = nui.ctx();
					ui.m_self["dispatcher"] = std::ref(m_scene->m_dispatcher);

					ui.m_update = ui.m_self["do_ui"];
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

		void World::destruct_nui(entt::registry& registry, entt::entity entity)
		{
			auto& ui = registry.get<components::UIScript>(entity);
			if (ui.m_self.valid())
			{
				ui.m_self.abandon();
			}
		}
	} // namespace core
} // namespace galaxy