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
			auto& em = ServiceLocator<meta::EntityMeta>::ref();

			if (registry.valid(entity))
			{
				m_json = em.serialize_entity(entity, registry);
			}
		}

		void Prefab::from_json(const nlohmann::json& json)
		{
			if (json.contains("components"))
			{
				m_json = json;
			}
		}

		entt::entity Prefab::to_entity(entt::registry& registry) const
		{
			auto& em = ServiceLocator<meta::EntityMeta>::ref();
			return em.deserialize_entity(m_json, registry);
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
