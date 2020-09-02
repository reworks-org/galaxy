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
	/// Shorthand for component factory map.
	///
	using ComponentFactory = std::unordered_map<std::string, std::function<void(const sr::Entity, const nlohmann::json&)>>;

	///
	/// The World class. Contains the entities and systems and other library stuff, like the main lua state.
	///
	class World final : public sr::Manager
	{
	public:
		///
		/// Constructor.
		///
		World();

		///
		/// Copy constructor.
		///
		World(const World&) noexcept = delete;

		///
		/// Move constructor.
		///
		World(World&&) noexcept = delete;

		///
		/// Copy assignment operator.
		///
		World& operator=(const World&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		World& operator=(World&&) noexcept = delete;

		///
		/// Destructor.
		///
		~World();

		///
		/// \brief Create an entity from a JSON file.
		///
		/// If your using this make sure you have called register_component().
		///
		/// \param file Filepath to the json.
		///
		/// \return Created entity.
		///
		const sr::Entity create_from_json(std::string_view file);

		///
		/// Registers a component definition.
		///
		/// \param name Name of component class in string format i.e. "AnimationComponent".
		///
		template<pr::is_class Component>
		void register_component(std::string_view name);

	private:
		///
		/// Used to allow for component creation without having to know the compile time type.
		///
		ComponentFactory m_component_factory;
	};

	template<pr::is_class Component>
	inline void World::register_component(std::string_view name)
	{
		// Make sure there are no duplicate components for the hashmap before registering.
		const auto str = static_cast<std::string>(name);
		if (m_component_factory.contains(str))
		{
			PL_LOG(PL_WARNING, "Attempted to register duplicate component: {0}.", str);
		}
		else
		{
			m_component_factory.emplace(name, [&](const sr::Entity e, const nlohmann::json& json) {
				create_component<Component>(e, json);
			});
		}
	}
} // namespace galaxy

#endif