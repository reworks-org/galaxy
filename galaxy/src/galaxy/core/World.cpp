///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/UIScript.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"
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
			, m_scene {scene}
			, m_rendersystem_index {0}
		{
			// Handle on_* events.
			m_registry.on_construct<components::Script>().connect<&World::construct_script>(this);
			m_registry.on_destroy<components::Script>().connect<&World::destruct_script>(this);
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
			m_scene = nullptr;
		}

		entt::entity World::create()
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
			const auto  entity     = m_registry.create();
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

			m_registry.emplace<flags::Disabled>(entity);

			if (!m_registry.all_of<components::Tag>(entity))
			{
				auto& tag = m_registry.emplace<components::Tag>(entity);
				tag.m_tag = "Untagged";
			}

			return entity;
		}

		void World::update()
		{
			for (auto i = 0; i < m_systems.size(); i++)
			{
				m_systems[i]->update(m_scene);
			}
		}

		void World::only_update_rendering()
		{
			m_systems[m_rendersystem_index]->update(m_scene);
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

			for (const auto& [entity] : m_registry.view<entt::entity>(entt::exclude<flags::DenySerialization>).each())
			{
				json["entities"].push_back(em.serialize_entity(entity, m_registry));
			}

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
