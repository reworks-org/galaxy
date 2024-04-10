///
/// Registry.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <glm/trigonometric.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/resource/Prefabs.hpp"

#include "Registry.hpp"

namespace galaxy
{
	namespace core
	{
		Registry::Registry()
		{
			// Handle on_* events.
			m_entt.on_construct<components::RigidBody>().connect<&Registry::construct_rigidbody>(this);
			m_entt.on_destroy<components::RigidBody>().connect<&Registry::destroy_rigidbody>(this);
			m_entt.on_construct<components::Script>().connect<&Registry::construct_script>(this);
			m_entt.on_destroy<components::Script>().connect<&Registry::destruct_script>(this);
			m_entt.on_construct<flags::Disabled>().connect<&Registry::disable_entity>(this);
			m_entt.on_destroy<flags::Disabled>().connect<&Registry::enable_entity>(this);

			// Handle incompatible components.
			m_entt.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Circle>>();
			m_entt.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Ellipse>>();
			m_entt.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Point>>();
			m_entt.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Polygon>>();
			m_entt.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Polyline>>();
			m_entt.on_construct<components::Sprite>().connect<&entt::registry::remove<components::Text>>();

			m_entt.on_construct<components::Text>().connect<&entt::registry::remove<components::Circle>>();
			m_entt.on_construct<components::Text>().connect<&entt::registry::remove<components::Ellipse>>();
			m_entt.on_construct<components::Text>().connect<&entt::registry::remove<components::Point>>();
			m_entt.on_construct<components::Text>().connect<&entt::registry::remove<components::Polygon>>();
			m_entt.on_construct<components::Text>().connect<&entt::registry::remove<components::Polyline>>();
			m_entt.on_construct<components::Text>().connect<&entt::registry::remove<components::Sprite>>();
			m_entt.on_construct<components::Text>().connect<&entt::registry::remove<components::Animated>>();

			m_entt.on_construct<components::Circle>().connect<&entt::registry::remove<components::Ellipse>>();
			m_entt.on_construct<components::Circle>().connect<&entt::registry::remove<components::Point>>();
			m_entt.on_construct<components::Circle>().connect<&entt::registry::remove<components::Polygon>>();
			m_entt.on_construct<components::Circle>().connect<&entt::registry::remove<components::Polyline>>();
			m_entt.on_construct<components::Circle>().connect<&entt::registry::remove<components::Sprite>>();
			m_entt.on_construct<components::Circle>().connect<&entt::registry::remove<components::Text>>();
			m_entt.on_construct<components::Circle>().connect<&entt::registry::remove<components::Animated>>();

			m_entt.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Circle>>();
			m_entt.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Point>>();
			m_entt.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Polygon>>();
			m_entt.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Polyline>>();
			m_entt.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Sprite>>();
			m_entt.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Text>>();
			m_entt.on_construct<components::Ellipse>().connect<&entt::registry::remove<components::Animated>>();

			m_entt.on_construct<components::Point>().connect<&entt::registry::remove<components::Circle>>();
			m_entt.on_construct<components::Point>().connect<&entt::registry::remove<components::Ellipse>>();
			m_entt.on_construct<components::Point>().connect<&entt::registry::remove<components::Polygon>>();
			m_entt.on_construct<components::Point>().connect<&entt::registry::remove<components::Polyline>>();
			m_entt.on_construct<components::Point>().connect<&entt::registry::remove<components::Sprite>>();
			m_entt.on_construct<components::Point>().connect<&entt::registry::remove<components::Text>>();
			m_entt.on_construct<components::Point>().connect<&entt::registry::remove<components::Animated>>();

			m_entt.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Circle>>();
			m_entt.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Ellipse>>();
			m_entt.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Point>>();
			m_entt.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Polyline>>();
			m_entt.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Sprite>>();
			m_entt.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Text>>();
			m_entt.on_construct<components::Polygon>().connect<&entt::registry::remove<components::Animated>>();

			m_entt.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Circle>>();
			m_entt.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Ellipse>>();
			m_entt.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Point>>();
			m_entt.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Polygon>>();
			m_entt.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Sprite>>();
			m_entt.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Text>>();
			m_entt.on_construct<components::Polyline>().connect<&entt::registry::remove<components::Animated>>();

			// Handle rendercommand creation.
			m_entt.on_construct<components::Sprite>().connect<&Registry::construct_rendercommand<components::Sprite>>(this);
			m_entt.on_construct<components::Text>().connect<&Registry::construct_rendercommand<components::Text>>(this);
			m_entt.on_construct<components::Circle>().connect<&Registry::construct_rendercommand<components::Circle>>(this);
			m_entt.on_construct<components::Ellipse>().connect<&Registry::construct_rendercommand<components::Ellipse>>(this);
			m_entt.on_construct<components::Point>().connect<&Registry::construct_rendercommand<components::Point>>(this);
			m_entt.on_construct<components::Polygon>().connect<&Registry::construct_rendercommand<components::Polygon>>(this);
			m_entt.on_construct<components::Polyline>().connect<&Registry::construct_rendercommand<components::Polyline>>(this);
		}

		Registry::~Registry()
		{
		}

		entt::entity Registry::create()
		{
			const auto entity = m_entt.create();

			m_entt.emplace<flags::Disabled>(entity);

			auto& tag = m_entt.emplace<components::Tag>(entity);
			tag.m_tag = "Untagged";

			return entity;
		}

		entt::entity Registry::create_from_prefab(const std::string& name)
		{
			auto& prefabs = core::ServiceLocator<resource::Prefabs>::ref();
			if (prefabs.has(name))
			{
				return prefabs.get(name)->to_entity(m_entt);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to load missing prefab '{0}'.", name);
				return entt::null;
			}
		}

		bool Registry::is_valid(const entt::entity entity)
		{
			auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

			for (const auto& hash : em.get_validation_list())
			{
				if (!(em.get_validations().at(hash)(entity, m_entt)))
				{
					return false;
				}
			}

			return true;
		}

		void Registry::update(b2World& b2World)
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

				rigidbody->m_body = b2World.CreateBody(&def);

				b2PolygonShape shape;
				shape.SetAsBox(rigidbody->m_shape.x, rigidbody->m_shape.y);

				b2FixtureDef fixture;
				fixture.shape                = &shape;
				fixture.density              = rigidbody->m_density;
				fixture.friction             = rigidbody->m_friction;
				fixture.restitution          = rigidbody->m_restitution;
				fixture.restitutionThreshold = rigidbody->m_restitution_threshold;

				rigidbody->m_body->CreateFixture(&fixture);
				rigidbody->m_world = &b2World;
			}

			m_bodies_to_construct.clear();
		}

		void Registry::clear()
		{
			m_bodies_to_construct.clear();
			m_entt.clear();
		}

		void Registry::construct_rigidbody(entt::registry& registry, entt::entity entity)
		{
			auto transform = registry.try_get<components::Transform>(entity);
			if (!transform)
			{
				transform = &registry.emplace<components::Transform>(entity);
			}

			m_bodies_to_construct.emplace_back(&registry.get<components::RigidBody>(entity), transform);
		}

		void Registry::destroy_rigidbody(entt::registry& registry, entt::entity entity)
		{
			auto& rigidbody = registry.get<components::RigidBody>(entity);

			if (rigidbody.m_body != nullptr)
			{
				rigidbody.m_world->DestroyBody(rigidbody.m_body);

				rigidbody.m_body  = nullptr;
				rigidbody.m_world = nullptr;
			}
		}

		void Registry::construct_script(entt::registry& registry, entt::entity entity)
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

		void Registry::destruct_script(entt::registry& registry, entt::entity entity)
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

		void Registry::enable_entity(entt::registry& registry, entt::entity entity)
		{
			auto rb = registry.try_get<components::RigidBody>(entity);
			if (rb)
			{
				rb->m_body->SetEnabled(true);
			}
		}

		void Registry::disable_entity(entt::registry& registry, entt::entity entity)
		{
			auto rb = registry.try_get<components::RigidBody>(entity);
			if (rb)
			{
				rb->m_body->SetEnabled(false);
			}
		}
	} // namespace core
} // namespace galaxy
