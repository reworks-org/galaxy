///
/// Prefab.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_PREFAB_HPP_
#define GALAXY_CORE_PREFAB_HPP_

#include <entt/fwd.hpp>
#include <robin_hood.h>
#include <nlohmann/json.hpp>

namespace galaxy
{
	namespace core
	{
		///
		/// Preconstructed entity definition that can be loaded at any time.
		///
		class Prefab final
		{
		  public:
			///
			/// Entity constructor.
			///
			/// \param entity Entity to use.
			/// \param registry Registry entity and its components belong to.
			///
			Prefab(entt::entity entity, entt::registry& registry);

			///
			/// JSON constructor.
			///
			/// \param json JSON data to construct prefab from.
			///
			Prefab(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Prefab();

			///
			/// Creates the prefab from an entity.
			///
			/// \param entity Entity to use.
			/// \param registry Registry entity and its components belong to.
			///
			void from_entity(entt::entity entity, entt::registry& registry);

			///
			/// Creates the prefab from a json object.
			///
			/// \param json JSON data to construct prefab from.
			///
			void from_json(const nlohmann::json& json);

			///
			/// Creates an entity from this prefab.
			///
			/// \param registry Registry to create the entity from.
			///
			/// \return Newly created entity.
			///
			[[nodiscard]] entt::entity to_entity(entt::registry& registry);

			///
			/// Gets the json representation of this prefab.
			///
			/// \return Const reference from the json object.
			///
			[[nodiscard]] const nlohmann::json& to_json() const;

		  private:
			///
			/// \brief Constructor.
			///
			/// Deleted.
			///
			Prefab() = delete;

		  private:
			///
			/// Prefab as json data.
			///
			nlohmann::json m_json;

			///
			/// Maps the meta id of the component to the component data.
			///
			robin_hood::unordered_flat_map<entt::id_type, entt::any> m_data;
		};
	} // namespace core
} // namespace galaxy

#endif
