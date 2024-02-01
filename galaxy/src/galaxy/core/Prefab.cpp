///
/// Prefab.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/ZLib.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "Prefab.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26496)
#endif

namespace galaxy
{
	namespace core
	{
		Prefab::Prefab()
		{
		}

		Prefab::Prefab(entt::entity entity, entt::registry& registry)
		{
			from_entity(entity, registry);
		}

		Prefab::Prefab(const nlohmann::json& json)
		{
			from_json(json);
		}

		Prefab::Prefab(const Prefab& p)
		{
			this->m_json = p.m_json;
		}

		Prefab::Prefab(Prefab&& p)
		{
			this->m_json = std::move(p.m_json);
		}

		Prefab& Prefab::operator=(const Prefab& p)
		{
			if (this != &p)
			{
				this->m_json = p.m_json;
			}

			return *this;
		}

		Prefab& Prefab::operator=(Prefab&& p)
		{
			if (this != &p)
			{
				this->m_json = std::move(p.m_json);
			}

			return *this;
		}

		Prefab::~Prefab()
		{
		}

		bool Prefab::load(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto data = fs.read(file);
			if (!data.empty())
			{
				const auto base64       = math::decode_zlib(data);
				const auto decompressed = math::decode_base64(base64);

				from_json(nlohmann::json::parse(decompressed));
				return true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to read empty prefab '{0}'.", file);
				return false;
			}
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
