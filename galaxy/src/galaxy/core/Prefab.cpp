///
/// Prefab.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/meta/EntityMeta.hpp"

#include "Prefab.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26496)
#endif

namespace galaxy
{
	namespace core
	{
		Prefab::Prefab(entt::entity entity, entt::registry& registry)
		{
			from_entity(entity, registry);
		}

		Prefab::Prefab(const nlohmann::json& json)
		{
			from_json(json);
		}

		Prefab::~Prefab()
		{
		}

		void Prefab::from_entity(entt::entity entity, entt::registry& registry)
		{
			m_data.clear();

			auto& em = ServiceLocator<meta::EntityMeta>::ref();

			for (auto&& [id, storage] : registry.storage())
			{
				if (storage.contains(entity))
				{
					m_data[id] = em.copy_to_any(id, storage.value(entity));
				}
			}

			m_json = em.serialize_entity(entity, registry);
		}

		void Prefab::from_json(const nlohmann::json& json)
		{
			const auto& components = json.at("components");
			if (!components.empty())
			{
				auto& em = core::ServiceLocator<meta::EntityMeta>::ref();
				for (const auto& [key, value] : components.items())
				{
					m_data[em.get_typeid(key)] = em.any_from_json(key, value);
				}
			}
		}

		entt::entity Prefab::to_entity(entt::registry& registry)
		{
			auto& em = ServiceLocator<meta::EntityMeta>::ref();

			const auto entity = registry.create();
			for (auto&& [id, component] : m_data)
			{
				em.any_factory(em.get_type(id), entity, registry, component);
			}

			return entity;
		}

		const nlohmann::json& Prefab::to_json() const
		{
			return m_json;
		}
	} // namespace core
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif