///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/Point.hpp"
#include "galaxy/components/Line.hpp"
#include "galaxy/components/Circle.hpp"
#include "galaxy/components/BatchedSprite.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Text.hpp"

#include "galaxy/flags/Enabled.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "World.hpp"

namespace galaxy
{
	namespace core
	{
		World::World() noexcept
		    : m_next_id {0}
		{
			register_component<components::ShaderID>("ShaderID");
			register_component<components::Transform>("Transform");
			register_component<components::Renderable>("Renderable");
			register_component<components::Point>("Point");
			register_component<components::Line>("Line");
			register_component<components::Circle>("Circle");
			register_component<components::BatchedSprite>("BatchedSprite");
			register_component<components::Sprite>("Sprite");
			register_component<components::Text>("Text");
		}

		World::~World()
		{
			clear();
		}

		void World::update(const double dt)
		{
			for (const auto& ptr : m_systems)
			{
				ptr->update(dt);
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
			return m_entities.emplace_back(entity);
		}

		const ecs::Entity World::create(std::string_view debug_name)
		{
			const auto entity = create();
			assign_name(entity, debug_name);

			return entity;
		}

		const ecs::Entity World::create_from_json(std::string_view file)
		{
			// Makes sure the filepath is correct for the current platform.
			const auto entity   = create();
			nlohmann::json root = json::parse_from_disk(file);

			assign_name(entity, root.at("name"));
			nlohmann::json components = root.at("components");

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

			const bool enabled = root.at("enabled");
			if (enabled)
			{
				enable(entity);
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
			if (has(entity))
			{
				return m_flags[entity].test(flags::Enabled::value);
			}
		}

		void World::enable(const ecs::Entity entity)
		{
			set_flag<flags::Enabled>(entity);
		}

		void World::disable(const ecs::Entity entity)
		{
			unset_flag<flags::Enabled>(entity);
		}

		const bool World::assign_name(const ecs::Entity entity, std::string_view debug_name) noexcept
		{
			if (has(entity))
			{
				const auto str = static_cast<std::string>(debug_name);
				if (!m_debug_names.contains(str))
				{
					m_debug_names.emplace(str, entity);
					return true;
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Attempted to add duplicate name: {0}.", str);
					return false;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to use invalid entity to assign name: {0}.", debug_name);
				return false;
			}
		}

		ecs::Entity World::find_from_name(std::string_view debug_name) noexcept
		{
			const auto str = static_cast<std::string>(debug_name);
			if (m_debug_names.contains(str))
			{
				return m_debug_names[str];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Cannot find entity to get with a name of: {0}.", str);
				return 0;
			}
		}

		void World::clear()
		{
			m_next_id = 0;

			m_entities.clear();
			m_invalid_entities.clear();
			m_flags.clear();
			m_debug_names.clear();
			m_data.clear();
			m_systems.clear();
			m_component_factory.clear();
		}

		const robin_hood::unordered_map<std::string, ecs::Entity>& World::get_debug_name_map() noexcept
		{
			return m_debug_names;
		}
	} // namespace core
} // namespace galaxy