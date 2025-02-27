///
/// EntityFactory.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/Serializable.hpp"

#include "EntityFactory.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26496)
#endif

namespace galaxy
{
	namespace meta
	{
		void EntityFactory::json_factory(const std::string& type, const entt::entity entity, entt::registry& registry, const nlohmann::json& json)
		{
			if (m_json_factory.contains(type))
			{
				m_json_factory[type](entity, registry, json);
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create component {0} from json factory.", type);
			}
		}

		entt::any EntityFactory::any_from_json(const std::string& type, const nlohmann::json& json)
		{
			return m_json_any_factory[type](json);
		}

		nlohmann::json EntityFactory::serialize_entity(const entt::entity entity, entt::registry& registry)
		{
			nlohmann::json json = nlohmann::json::object();
			json["components"]  = nlohmann::json::object();

			for (auto&& [id, storage] : registry.storage())
			{
				if (storage.contains(entity))
				{
					SerializationData data = m_serialize_factory[get_type(id)](storage.value(entity));
					if (!data.name.empty())
					{
						json["components"][data.name] = data.json;
					}
				}
			}

			return json;
		}

		entt::entity EntityFactory::deserialize_entity(const nlohmann::json& json, entt::registry& registry)
		{
			const auto  entity     = registry.create();
			const auto& components = json.at("components");

			if (!components.empty())
			{
				for (const auto& [key, value] : components.items())
				{
					// Use the assign function to create components for entities without having to know the type.
					json_factory(key, entity, registry, value);
				}
			}

			return entity;
		}

		const std::string& EntityFactory::get_type(const entt::id_type id)
		{
			return m_id_to_name.at(id);
		}

		entt::id_type EntityFactory::get_typeid(const std::string& name)
		{
			return m_name_to_id.at(name);
		}

		EntityFactory::Validations& EntityFactory::get_validations()
		{
			return m_validations;
		}

		const std::vector<entt::id_type>& EntityFactory::get_validation_list() const
		{
			return m_validations_to_run;
		}
	} // namespace meta
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
