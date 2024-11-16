///
/// EntityMeta.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_ENTITYMETA_HPP_
#define GALAXY_META_ENTITYMETA_HPP_

#include <ankerl/unordered_dense.h>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// Handles entity meta (de)serialization.
		///
		class EntityMeta final
		{
			///
			/// Data needed to serialize an entity.
			///
			struct SerializationData final
			{
				///
				/// Name of the component.
				///
				std::string name;

				///
				/// Component data as a json object.
				///
				nlohmann::json json;
			};

			using Validations          = ankerl::unordered_dense::map<entt::id_type, std::move_only_function<bool(const entt::entity, entt::registry&)>>;
			using ComponentJSONFactory = ankerl::unordered_dense::map<std::string, std::move_only_function<void(const entt::entity, entt::registry&, const nlohmann::json&)>>;
			using AnyJSONFactory       = ankerl::unordered_dense::map<std::string, std::move_only_function<entt::any(const nlohmann::json&)>>;
			using SerializeFactory     = ankerl::unordered_dense::map<std::string, std::move_only_function<EntityMeta::SerializationData(void*)>>;

		public:
			///
			/// Defines a dependency validation for components.
			///
			/// \tparam ToValidate The component to validate.
			/// \tparam Dependencies The components required by ToValidate.
			///
			template<valid_component ToValidate, valid_component... Dependencies>
			void register_dependencies();

			///
			/// Registers a component definition.
			///
			/// \tparam Component A valid component type is required. Must be move assignable/constructible and a class with a json constructor.
			///
			/// \param name Name of component class in string format i.e. "Transform" or "Tag".
			///
			template<valid_component Component>
			void register_component(const std::string& name);

			///
			/// Construct component from json and assign to provided entity and registry.
			///
			/// \param type Type of component as a string.
			/// \param entity Entity to assign component to.
			/// \param registry Registry that the entity belong to and where to create the component.
			/// \param json Component data as a json object.
			///
			void json_factory(const std::string& type, const entt::entity entity, entt::registry& registry, const nlohmann::json& json);

			///
			/// Construct an entt::any from a json object.
			///
			/// \param type Component type as a string.
			/// \param json Component data as a json object.
			///
			/// \return Newly constructed entt::any.
			///
			[[nodiscard]]
			entt::any any_from_json(const std::string& type, const nlohmann::json& json);

			///
			/// Serialise a single entity.
			///
			/// \param entity Entity to serialize.
			/// \param registry Registry entity belongs to.
			///
			/// \return JSON entity data.
			///
			[[nodiscard]]
			nlohmann::json serialize_entity(const entt::entity entity, entt::registry& registry);

			///
			/// \brief Create an entity from a JSON object.
			///
			/// If your using this make sure you have called register_component().
			///
			/// \param json Preloaded JSON object.
			/// \param registry Registry entity belongs to.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]]
			entt::entity deserialize_entity(const nlohmann::json& json, entt::registry& registry);

			///
			/// Get a string representation of an entity type id.
			///
			/// \param id Entt meta type id of a component.
			///
			/// \return String name of component.
			///
			[[nodiscard]]
			const std::string& get_type(const entt::id_type id);

			///
			/// Get an entity type id from a string.
			///
			/// \param name String name of component.
			///
			/// \return Entt meta type id of a component.
			///
			[[nodiscard]]
			entt::id_type get_typeid(const std::string& name);

			///
			/// Get a list of configurations.
			///
			/// \return Reference to validations map.
			///
			[[nodiscard]]
			Validations& get_validations();

			///
			/// Get a list of validations to run.
			///
			/// \return Const reference to list of validations to run.
			///
			[[nodiscard]]
			const meta::vector<entt::id_type>& get_validation_list() const;

		private:
			///
			/// Maps an entt type id to a string.
			///
			ankerl::unordered_dense::map<entt::id_type, std::string> m_id_to_name;

			///
			/// Maps a string name to an entt id.
			///
			ankerl::unordered_dense::map<std::string, entt::id_type> m_name_to_id;

			///
			/// Used to allow for component creation from json.
			///
			ComponentJSONFactory m_json_factory;

			///
			/// Creates an entt::any from a json component data.
			///
			AnyJSONFactory m_json_any_factory;

			///
			/// Serializes an entity from a void* pointer.
			///
			SerializeFactory m_serialize_factory;

			///
			/// Stores validation configurations.
			///
			Validations m_validations;

			///
			/// Validations to run upon request.
			///
			meta::vector<entt::id_type> m_validations_to_run;
		};

		template<valid_component ToValidate, valid_component... Dependencies>
		inline void EntityMeta::register_dependencies()
		{
			const auto hash = entt::type_id<ToValidate>().hash();
			if (!m_validations.contains(hash))
			{
				m_validations[hash] = [this](const entt::entity entity, entt::registry& registry) -> bool {
					const auto* component = registry.try_get<ToValidate>(entity);
					if (component)
					{
						return registry.all_of<Dependencies...>(entity);
					}
					else
					{
						// If we dont have component needing to be validated, then entity is valid.
						return true;
					}
				};

				m_validations_to_run.push_back(hash);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to register duplicate validation of a component dependency.");
			}
		}

		template<valid_component Component>
		inline void EntityMeta::register_component(const std::string& name)
		{
			if (!m_json_factory.contains(name))
			{
				const auto hash = entt::type_id<Component>().hash();
				m_id_to_name.emplace(hash, name);
				m_name_to_id.emplace(name, hash);

				if constexpr (std::constructible_from<Component, const nlohmann::json&>)
				{
					m_json_factory.emplace(name, [](const entt::entity entity, entt::registry& registry, const nlohmann::json& json) {
						registry.emplace<Component>(entity, json);
					});

					m_json_any_factory.emplace(name, [](const nlohmann::json& json) -> entt::any {
						return entt::make_any<Component>(json);
					});
				}

				if constexpr (std::derived_from<Component, fs::Serializable>)
				{
					m_serialize_factory.emplace(name, [name](void* component) -> SerializationData {
						return SerializationData {.name = name, .json = static_cast<Component*>(component)->serialize()};
					});
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to register duplicate component of type '{0}'.", name);
			}
		}
	} // namespace meta
} // namespace galaxy

#endif
