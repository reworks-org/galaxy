///
/// Prefab.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_PREFAB_HPP_
#define GALAXY_CORE_PREFAB_HPP_

#include <entt/fwd.hpp>
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
			/// Constructor.
			///
			Prefab();

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
			/// Copy constructor.
			///
			Prefab(const Prefab&);

			///
			/// Move constructor.
			///
			Prefab(Prefab&&);

			///
			/// Copy assignment operator.
			///
			Prefab& operator=(const Prefab&);

			///
			/// Move assignment operator.
			///
			Prefab& operator=(Prefab&&);

			///
			/// Destructor.
			///
			~Prefab();

			///
			/// Loads from a json file on disk.
			///
			/// \param file Json file to read from disk.
			///
			[[nodiscard]]
			bool load(const std::string& file);

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
			[[nodiscard]]
			entt::entity to_entity(entt::registry& registry) const;

			///
			/// Gets the json representation of this prefab.
			///
			/// \return Const reference from the json object.
			///
			[[nodiscard]]
			const nlohmann::json& to_json() const;

		private:
			///
			/// Prefab as json data.
			///
			nlohmann::json m_json;
		};
	} // namespace core
} // namespace galaxy

#endif
