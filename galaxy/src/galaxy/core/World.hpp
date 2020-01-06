///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_WORLD_HPP_
#define GALAXY_WORLD_HPP_

#include <pl/Log.hpp>
#include <nlohmann/json_fwd.hpp>
#include <solar/entity/Manager.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// The World class. Contains the entities and systems and other library stuff, like the main lua state.
	///
	class World final
	{
		///
		/// Shorthand for component factory map.
		///
		using ComponentFactory = std::unordered_map<std::string, std::function<void(const sr::Entity, const nlohmann::json&)>>;

	public: 
		///
		/// Constructor.
		///
		World();

		///
		/// Destructor.
		///
		~World() noexcept;

		///
		/// Register an entity.
		///
		/// \param def Entity definition from JSON in VFS.
		///
		/// \return Value on entity created.
		///
		sr::Entity createEntity(const std::string& def);

		///
		/// Create multiple entitys from a single definition file.
		///
		/// \param def Entity definition from JSON in VFS, containing multiple entities.
		///
		void createEntities(const std::string& def);

		///
		/// Create a batch of entities that use the same definition.
		///
		/// \param def Entity definition from JSON in VFS.
		///
		void createDuplicateEntities(const unsigned int count, const std::string& def);
		
		///
		/// Registers a component definition.
		///
		/// \param name Name of component class in string format i.e. "AnimationComponent".
		///
		template<typename Component>
		void registerComponent(const std::string& name);

		///
		/// Main update method for ECS.
		///
		/// \param dt Time difference from update loop.
		/// 
		void update(const double dt);
		
		///
		/// Get reference to internal ECS manager.
		///
		sr::Manager& manager() noexcept;

	private:
		///
		/// ECS manager and main class.
		///
		sr::Manager m_manager;

		///
		/// Used to allow for component creation without having to know the compile time type.
		///
		ComponentFactory m_componentFactory;
	};

	template<typename Component>
	inline void World::registerComponent(const std::string& name)
	{
		// Make sure there are no duplicate components for the hashmap before registering.
		if (m_componentFactory.find(name) != m_componentFactory.end())
		{
			PL_LOG(pl::Log::Level::WARNING, "Attempted to register duplicate component: " + name);
		}
		else
		{
			m_componentFactory.emplace(name, [this](const sr::Entity e, const nlohmann::json& json)
			{
				m_manager.add<Component>(e, json);
			});
		}
	}
}

#endif