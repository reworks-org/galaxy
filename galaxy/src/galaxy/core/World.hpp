///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WORLD_HPP_
#define GALAXY_CORE_WORLD_HPP_

#include <entt/core/ident.hpp>
#include <entt/entity/registry.hpp>
#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/meta/Concepts.hpp"
#include "galaxy/systems/System.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Provides an additional set of functionality around an entt::registry.
		///
		class World final : public fs::Serializable
		{
			using SystemContainer  = std::vector<std::shared_ptr<systems::System>>;
			using ComponentFactory = robin_hood::unordered_flat_map<std::string, std::function<void(const entt::entity, const nlohmann::json&)>>;

		public:
			///
			/// Constructor.
			///
			World() noexcept;

			///
			/// Destructor.
			///
			~World() noexcept;

			///
			/// \brief Create an entity from a JSON file.
			///
			/// If your using this make sure you have called register_component().
			///
			/// \param file Filepath to the json.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]] entt::entity create_from_file(std::string_view file);

			///
			/// \brief Create an entity from a JSON object.
			///
			/// If your using this make sure you have called register_component().
			///
			/// \param json Preloaded JSON object.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]] entt::entity create_from_obj(const nlohmann::json& json);

			///
			/// Registers a component definition.
			///
			/// \tparam Component A valid component type is required. Must be move assignable/constructible and a class with a json constructor.
			///
			/// \param name Name of component class in string format i.e. "Transform" or "Tag".
			///
			template<meta::valid_component Component>
			void register_component(const std::string& name) noexcept;

			///
			/// \brief Add a system to the manager.
			///
			/// \tparam System Type of system to create.
			/// \tparam Args Constructor arguments for system.
			///
			/// Systems will be updated in the order in which they are created.
			///
			/// \param args Constructor arguments for system.
			///
			/// \return Pointer to the system.
			///
			template<meta::is_system System, typename... Args>
			[[maybe_unused]] std::shared_ptr<System> create_system(Args&&... args);

			///
			/// Update systems.
			///
			/// \param layer Non-owning pointer to current layer.
			///
			void update_systems(state::Layer* layer);

			///
			/// Clears all system, component registry, and entity data.
			///
			void clear() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			World(const World&) = delete;

			///
			/// Move constructor.
			///
			World(World&&) = delete;

			///
			/// Copy assignment operator.
			///
			World& operator=(const World&) = delete;

			///
			/// Move assignment operator.
			///
			World& operator=(World&&) = delete;

		public:
			///
			/// The main entt entity registry.
			///
			entt::registry m_registry;

		private:
			///
			/// Stores systems.
			///
			SystemContainer m_systems;

			///
			/// Used to allow for component creation without having to know the compile time type.
			///
			ComponentFactory m_component_factory;
		};

		template<meta::valid_component Component>
		inline void World::register_component(const std::string& name) noexcept
		{
			if (!m_component_factory.contains(name))
			{
				m_component_factory.emplace(name, [this](const entt::entity entity, const nlohmann::json& json) {
					this->m_registry.emplace<Component>(entity, json);
				});
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to register duplicate component of type '{0}'.", name);
			}
		}

		template<meta::is_system System, typename... Args>
		inline std::shared_ptr<System> World::create_system(Args&&... args)
		{
			auto ptr = std::make_shared<System>(std::forward<Args>(args)...);
			m_systems.push_back(std::static_pointer_cast<systems::System>(ptr));

			return ptr;
		}
	} // namespace core
} // namespace galaxy

#endif