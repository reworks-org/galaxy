///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENE_SCENE_HPP_
#define GALAXY_SCENE_SCENE_HPP_

#include "galaxy/systems/System.hpp"

namespace galaxy
{
	///
	/// \brief Represents a scene in a game.
	///
	/// Scenes should be logically grouped -> i.e. a map, player data + ui, battle, menu, etc.
	/// Each scene is an independant collection of systems, but not entities.
	///
	class Scene final
	{
	public:
		///
		/// Name constructor.
		///
		/// \param name Name of the scene for debug / editor purposes.
		///
		Scene(const std::string& name) noexcept;

		///
		/// Destructor.
		///
		~Scene();

		///
		/// When scene is pushed to the stack.
		///
		void load();

		///
		/// When scene is popped from the stack.
		///
		void unload();

		///
		/// Process events and updates.
		///
		/// \param registry Entities to process.
		///
		void update(Registry& registry);

		///
		/// Render scene.
		///
		void render();

		///
		/// \brief Add a system to operate on entities in this scene.
		///
		/// Scene is called in order of adding. So i.e. if you add anim then render, systems are called in that order.
		///
		/// \param system Name of system to add to this scene.
		///
		void add_system(const std::string& system);

		///
		/// Remove all scene data.
		///
		void clear();

		///
		/// Get scene name.
		///
		/// \return Const string.
		///
		[[nodiscard]]
		const std::string& name() const noexcept;

	private:
		///
		/// Constructor.
		///
		Scene() = delete;

	private:
		///
		/// Scene name for debug purposes.
		///
		std::string m_name;

		///
		/// List of systems to process.
		///
		std::vector<System> m_systems;
	};
} // namespace galaxy

/*
///
/// Update ui.
///
void update_ui();

			///
			/// Only update rendering.
			///
			void only_update_rendering();

///
/// Loads an LDTK world for this scene.
///
/// \param file .ldtk project file to load.
///
/// \return True if loaded successfully.
///
[[nodiscard]]
bool load_world(const std::string& file);
///
/// Serializes object.
///
/// \return JSON object containing data to write out.
///
[[nodiscard]]
nlohmann::json serialize() override;

///
/// Deserializes from object.
///
/// \param json Json object to retrieve data from.
///
void deserialize(const nlohmann::json& json) override;
///
/// Camera.
///
graphics::Camera m_camera;

///
/// Scene event handler.
///
entt::dispatcher m_dispatcher;

///
/// Box2D physics world.
///
b2World m_b2world;

///
/// LDTK world.
///
map::World m_world;

///
/// Box2D world velocity iterations.
///
int m_velocity_iterations;

///
/// Box2d world position iterations.
///
int m_position_iterations;

///
			/// Rendersystem index.
			///
			std::size_t m_rendersystem_index;
*/

#endif
