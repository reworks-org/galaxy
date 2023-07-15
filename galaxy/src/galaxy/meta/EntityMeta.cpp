///
/// EntityMeta.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/Serializable.hpp"

#include "EntityMeta.hpp"

namespace galaxy
{
	namespace meta
	{
		void EntityMeta::json_factory(const std::string& type, const entt::entity entity, entt::registry& registry, const nlohmann::json& json)
		{
			m_json_factory[type](entity, registry, json);
		}

		void EntityMeta::any_factory(const std::string& type, const entt::entity entity, entt::registry& registry, entt::any& any)
		{
			m_any_factory[type](entity, registry, any);
		}

		entt::any EntityMeta::any_from_json(const std::string& type, const nlohmann::json& json)
		{
			return m_json_any_factory[type](json);
		}

		nlohmann::json EntityMeta::serialize_entity(const entt::entity entity, entt::registry& registry)
		{
			nlohmann::json json = nlohmann::json::object();
			json["components"]  = nlohmann::json::object();

			for (const auto&& [id, storage] : registry.storage())
			{
				if (storage.contains(entity))
				{
					const auto& name         = get_type(id);
					auto component           = static_cast<fs::Serializable*>(storage.value(entity));
					json["components"][name] = component->serialize();
				}
			}

			return json;
		}

		entt::any EntityMeta::copy_to_any(const entt::id_type id, void* component)
		{
			return m_ptr_factory[get_type(id)](component);
		}

		const std::string& EntityMeta::get_type(const entt::id_type id)
		{
			return m_id_to_name.at(id);
		}

		entt::id_type EntityMeta::get_typeid(const std::string& name)
		{
			return m_name_to_id.at(name);
		}

		const EntityMeta::Validations& EntityMeta::get_validations() const
		{
			return m_validations;
		}

		const meta::vector<entt::id_type>& EntityMeta::get_validation_list() const
		{
			return m_validations_to_run;
		}
	} // namespace meta
} // namespace galaxy