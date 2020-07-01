///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_WORLD_HPP_
#define GALAXY_WORLD_HPP_

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
	class World final : public sr::Manager
	{
		///
		/// Shorthand for component factory map.
		///
		using ComponentFactory = std::unordered_map<std::string, std::function<void(const sr::Entity, const nlohmann::json&)>>;

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
		/// If your using this make sure you have called registerComponent() and defineJSONConstruction().
		///
		/// \param file Filepath to the json.
		///
		/// \return Created entity.
		///
		const sr::Entity createFromJSON(const std::string& file) noexcept;

		///
		/// Registers a component definition.
		///
		/// \param name Name of component class in string format i.e. "AnimationComponent".
		///
		template<typename Component>
		void registerComponent(const std::string& name) noexcept;

		///
		///
		///
		void serialize() noexcept;

		///
		///
		///
		void serialize(const sr::Entity entity) noexcept;

	private:
		///
		/// Used to allow for component creation without having to know the compile time type.
		///
		ComponentFactory m_componentFactory;
	};

	template<typename Component>
	inline void World::registerComponent(const std::string& name) noexcept
	{
		// Make sure there are no duplicate components for the hashmap before registering.
		if (m_componentFactory.find(name) != m_componentFactory.end())
		{
			PL_LOG(PL_WARNING, "Attempted to register duplicate component: " + name);
		}
		else
		{
			m_componentFactory.emplace(name, [this](const sr::Entity e, const nlohmann::json& json)
			{
				this->add<Component>(e, json);
			}) noexcept;
		}
	}
}

#endif