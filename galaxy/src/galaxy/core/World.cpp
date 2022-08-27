///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/scripting/JSON.hpp"

#include "World.hpp"

namespace galaxy
{
	namespace core
	{
		World::World() noexcept
			: Serializable {}
		{
			register_component<components::DrawShader>("DrawShader");
			register_component<components::Flag>("Flag");
			register_component<components::Primitive>("Primitive");
			register_component<components::Sprite>("Sprite");
			register_component<components::Tag>("Tag");
			register_component<components::Text>("Text");
			register_component<components::Transform>("Transform");
		}

		World::~World() noexcept
		{
			clear();
		}

		entt::entity World::create_from_file(std::string_view file)
		{
			const auto root = json::parse_from_disk(file);
			if (root.has_value())
			{
				return create_from_obj(root.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load entity json '{0}'.", file);
				return entt::null;
			}
		}

		entt::entity World::create_from_obj(const nlohmann::json& json)
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

			return entity;
		}

		void World::update_systems(state::Layer* scene)
		{
			for (auto i = 0; i < m_systems.size(); i++)
			{
				m_systems[i]->update(scene);
			}
		}

		void World::clear() noexcept
		{
			m_systems.clear();
			m_component_factory.clear();
			m_registry.clear();
		}

		nlohmann::json World::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["entities"] = nlohmann::json::array();

			m_registry.each([&](const entt::entity entity) -> void {
				nlohmann::json entity_json = nlohmann::json::object();
				entity_json["components"]  = nlohmann::json::object();

				auto draw_shader = m_registry.try_get<components::DrawShader>(entity);
				if (draw_shader != nullptr)
				{
					entity_json["components"]["DrawShader"] = draw_shader->serialize();
				}

				auto flag = m_registry.try_get<components::Flag>(entity);
				if (flag != nullptr)
				{
					entity_json["components"]["Flag"] = flag->serialize();
				}

				auto primitive = m_registry.try_get<components::Primitive>(entity);
				if (primitive != nullptr)
				{
					entity_json["components"]["Primitive"] = primitive->serialize();
				}

				auto sprite = m_registry.try_get<components::Sprite>(entity);
				if (sprite != nullptr)
				{
					entity_json["components"]["Sprite"] = sprite->serialize();
				}

				auto tag = m_registry.try_get<components::Tag>(entity);
				if (tag != nullptr)
				{
					entity_json["components"]["Tag"] = tag->serialize();
				}

				auto text = m_registry.try_get<components::Text>(entity);
				if (text != nullptr)
				{
					entity_json["components"]["Text"] = text->serialize();
				}

				auto transform = m_registry.try_get<components::Transform>(entity);
				if (transform != nullptr)
				{
					entity_json["components"]["Transform"] = transform->serialize();
				}

				json["entities"].push_back(entity_json);
			});

			return json;
		}

		void World::deserialize(const nlohmann::json& json)
		{
			m_registry.clear();

			const auto& entity_json = json.at("entities");
			for (const auto& entity : entity_json)
			{
				create_from_obj(entity);
			}
		}
	} // namespace core
} // namespace galaxy