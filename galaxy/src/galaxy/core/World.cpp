///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>
#include <optional>

#include <nlohmann/json.hpp>

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/OnCollision.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"

#include "galaxy/flags/AllowSerialize.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "World.hpp"

namespace galaxy
{
	namespace core
	{
		World::World()
		    : Serializable {this}, m_next_id {0}
		{
			register_component<components::Animated>("Animated");
			register_component<components::BatchSprite>("BatchSprite");
			register_component<components::OnCollision>("OnCollision");
			register_component<components::Primitive2D>("Primitive2D");
			register_component<components::Renderable>("Renderable");
			register_component<components::RigidBody>("RigidBody");
			register_component<components::ShaderID>("ShaderID");
			register_component<components::Sprite>("Sprite");
			register_component<components::Tag>("Tag");
			register_component<components::Text>("Text");
			register_component<components::Transform>("Transform");
		}

		World::~World()
		{
			clear();
		}

		void World::update(const double dt)
		{
			for (const auto& pair : m_systems)
			{
				pair.second->update(*this, dt);
			}
		}

		const ecs::Entity World::create()
		{
			ecs::Entity entity = 0;
			if (!m_invalid_entities.empty())
			{
				entity = m_invalid_entities[m_invalid_entities.size() - 1];
				m_invalid_entities.pop_back();
			}
			else
			{
				entity = m_next_id++;
			}

			m_flags[entity] = {};
			m_entities.emplace_back(entity);

			return entity;
		}

		std::optional<ecs::Entity> World::create_from_json(std::string_view file)
		{
			const auto root = json::parse_from_disk(file);
			if (root == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file for entity: {0}.", file);
				return std::nullopt;
			}
			else
			{
				return std::make_optional(create_from_json_obj(root.value()));
			}
		}

		const ecs::Entity World::create_from_json_obj(const nlohmann::json& json)
		{
			const auto entity = create();

			nlohmann::json components = json.at("components");

			// Loop over components
			if (!components.empty())
			{
				for (auto& [key, value] : components.items())
				{
					// Use the assign function to create components for entities without having to know the type.
					m_component_factory[key](entity, value);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Created an entity with no components.");
			}

			const bool enabled = json.at("enabled");
			if (enabled)
			{
				enable(entity);
			}

			if (json.count("allow-serialize") > 0)
			{
				const bool allow_serialize = json.at("allow-serialize");
				if (allow_serialize)
				{
					set_flag<flags::AllowSerialize>(entity);
				}
			}

			return entity;
		}

		void World::destroy(const ecs::Entity entity)
		{
			m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());

			for (const auto& ptr : m_data)
			{
				ptr->remove(entity);
			}

			m_invalid_entities.push_back(entity);

			for (auto it = m_debug_names.begin(); it != m_debug_names.end();)
			{
				if (it->second == entity)
				{
					m_debug_names.erase(it++);
				}
				else
				{
					it++;
				}
			}

			m_flags.erase(entity);
		}

		const bool World::has(const ecs::Entity entity) noexcept
		{
			return (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end());
		}

		const bool World::is_enabled(const ecs::Entity entity)
		{
			return is_flag_set<flags::Enabled>(entity);
		}

		void World::enable(const ecs::Entity entity)
		{
			set_flag<flags::Enabled>(entity);
		}

		void World::disable(const ecs::Entity entity)
		{
			unset_flag<flags::Enabled>(entity);
		}

		void World::clear()
		{
			m_next_id = 0;

			m_entities.clear();
			m_invalid_entities.clear();
			m_flags.clear();
			m_debug_names.clear();

			for (auto& ptr : m_data)
			{
				ptr.reset();
			}
			m_data.clear();
		}

		nlohmann::json World::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["entities"] = nlohmann::json::array();
			for (const auto& entity : m_entities)
			{
				const auto allow_serialize = is_flag_set<flags::AllowSerialize>(entity);
				if (allow_serialize)
				{
					nlohmann::json entity_json = nlohmann::json::object();

					// Data
					// clang-format off

					entity_json["allow-serialize"] = allow_serialize;
					entity_json["enabled"] = is_enabled(entity);
					entity_json["components"] = nlohmann::json::object();

					auto [animated, batchsprite, oncollision, primitive2d, renderable, rigidbody, shaderid, sprite, tag, text, transform] = get_multi<
						components::Animated,
						components::BatchSprite,
						components::OnCollision,
						components::Primitive2D,
						components::Renderable,
						components::RigidBody,
						components::ShaderID,
						components::Sprite,
						components::Tag,
						components::Text,
						components::Transform>(entity);
					// clang-format on
					if (animated)
					{
						entity_json["components"]["Animated"] = animated->serialize();
					}

					if (batchsprite)
					{
						entity_json["components"]["BatchSprite"] = batchsprite->serialize();
					}

					if (oncollision)
					{
						entity_json["components"]["OnCollision"] = oncollision->serialize();
					}

					if (primitive2d)
					{
						entity_json["components"]["Primitive2D"] = primitive2d->serialize();
					}

					if (renderable)
					{
						entity_json["components"]["Renderable"] = renderable->serialize();
					}

					if (rigidbody)
					{
						entity_json["components"]["RigidBody"] = rigidbody->serialize();
					}

					if (shaderid)
					{
						entity_json["components"]["ShaderID"] = shaderid->serialize();
					}

					if (sprite)
					{
						entity_json["components"]["Sprite"] = sprite->serialize();
					}

					if (tag)
					{
						entity_json["components"]["Tag"] = tag->serialize();
					}

					if (text)
					{
						entity_json["components"]["Text"] = text->serialize();
					}

					if (transform)
					{
						entity_json["components"]["Transform"] = transform->serialize();
					}

					json["entities"].push_back(entity_json);
				}
			}

			return json;
		}

		void World::deserialize(const nlohmann::json& json)
		{
			clear();

			const auto entity_json = json.at("entities");
			for (const auto& obj : entity_json)
			{
				create_from_json_obj(obj);
			}
		}
	} // namespace core
} // namespace galaxy