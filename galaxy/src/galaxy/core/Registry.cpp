///
/// Registry.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/components/Tag.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/meta/EntityFactory.hpp"

#include "Registry.hpp"

namespace galaxy
{
	namespace core
	{
		Registry::Registry()
		{
			// Handle on_* events.
			/*m_entt.on_construct<components::RigidBody>().connect<&Registry::construct_rigidbody>(this);
			m_entt.on_destroy<components::RigidBody>().connect<&Registry::destroy_rigidbody>(this);
			m_entt.on_construct<components::Script>().connect<&Registry::construct_script>(this);
			m_entt.on_destroy<components::Script>().connect<&Registry::destruct_script>(this);*/
			m_entt.on_construct<flags::Disabled>().connect<&Registry::disable_entity>(this);
			m_entt.on_destroy<flags::Disabled>().connect<&Registry::enable_entity>(this);
		}

		Registry::Registry(Registry&& r)
		{
			// this->m_bodies_to_construct = std::move(r.m_bodies_to_construct);
			this->m_entt = std::move(r.m_entt);
		}

		Registry& Registry::operator=(Registry&& r)
		{
			if (this != &r)
			{
				// this->m_bodies_to_construct = std::move(r.m_bodies_to_construct);
				this->m_entt = std::move(r.m_entt);
			}

			return *this;
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

		/*entt::entity Registry::create_from_prefab(const std::string& name)
		{
			auto& prefabs = entt::locator<resource::Prefabs>::value();
			if (prefabs.has(name))
			{
				return prefabs.get(name)->to_entity(m_entt);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to load missing prefab '{0}'.", name);
				return entt::null;
			}
		}*/

		bool Registry::is_valid(const entt::entity entity)
		{
			auto& em = entt::locator<meta::EntityFactory>::value();

			for (const auto& hash : em.get_validation_list())
			{
				if (!(em.get_validations().at(hash)(entity, m_entt)))
				{
					return false;
				}
			}

			return true;
		}

		/*void Registry::update(b2World& b2World)
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
		}*/

		void Registry::clear()
		{
			// m_bodies_to_construct.clear();
			m_entt.clear();
		}

		/*void Registry::construct_rigidbody(entt::registry& registry, entt::entity entity)
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
			auto& state  = entt::locator<sol::state>::value();

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

		void Registry::construct_nui(entt::registry& registry, entt::entity entity)
		{
			auto& ui    = registry.get<components::GUI>(entity);
			auto& state = entt::locator<sol::state>::value();
			auto& nui   = entt::locator<ui::NuklearUI>::value();
			auto& fs    = entt::locator<fs::VirtualFileSystem>::value();

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

		void Registry::destruct_nui(entt::registry& registry, entt::entity entity)
		{
			auto& ui = registry.get<components::GUI>(entity);
			if (ui.m_self.valid())
			{
				ui.m_self.abandon();
			}
		}*/

		void Registry::enable_entity(entt::registry& registry, entt::entity entity)
		{
			/*auto rb = registry.try_get<components::RigidBody>(entity);
			if (rb)
			{
				rb->m_body->SetEnabled(true);
			}*/
		}

		void Registry::disable_entity(entt::registry& registry, entt::entity entity)
		{
			/*auto rb = registry.try_get<components::RigidBody>(entity);
			if (rb)
			{
				rb->m_body->SetEnabled(false);
			}*/
		}
	} // namespace core
} // namespace galaxy
