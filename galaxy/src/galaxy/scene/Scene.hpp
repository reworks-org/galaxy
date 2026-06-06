///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENE_SCENE_HPP_
#define GALAXY_SCENE_SCENE_HPP_

#include <entt/signal/dispatcher.hpp>
#include <nlohmann/json_fwd.hpp>
#include <SDL3/SDL_events.h>

#include "galaxy/state/State.hpp"
#include "galaxy/systems/SystemManager.hpp"

namespace galaxy
{
	///
	/// \brief Represents a scene in a game.
	///
	/// Scenes should be logically grouped -> i.e. a map, player data + ui, battle, menu, etc.
	/// Each scene is an independant collection of systems, but not entities.
	///
	class Scene : public State
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
		virtual ~Scene() noexcept;

		///
		/// When scene is pushed to the stack.
		///
		void on_push() override;

		///
		/// When scene is popped from the stack.
		///
		void on_pop() override;

		///
		/// Handle an event for a scene.
		///
		/// \param e Event that was triggered.
		///
		virtual void on_event(SDL_Event& event);

		///
		/// Process events and updates.
		///
		/// \param em Entities to process.
		///
		virtual void update(EntityManager& em);

		///
		/// Render scene.
		///
		virtual void render();

		///
		/// Serializes object.
		///
		/// \return JSON object containing data to be serialized.
		///
		[[nodiscard]]
		nlohmann::json serialize();

		///
		/// Deserializes from object.
		///
		/// \param json Json object to retrieve data from.
		///
		void deserialize(const nlohmann::json& json);

		///
		/// Get system manager.
		///
		/// \return Reference to systems for this specfic scene.
		///
		[[nodiscard]]
		SystemManager& sys_man() noexcept;

	private:
		///
		/// Constructor.
		///
		Scene() = delete;

		///
		/// Copy constructor.
		///
		Scene(const Scene&) = delete;

		///
		/// Copy assignment operator.
		///
		Scene& operator=(const Scene&) = delete;

	private:
		///
		/// Systems only used by this scene.
		///
		SystemManager m_sysman;

		///
		/// Scene event handler.
		///
		entt::dispatcher m_dispatcher;
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
