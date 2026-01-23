///
/// SceneManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENE_SCENEMANAGER_HPP_
#define GALAXY_SCENE_SCENEMANAGER_HPP_

#include "galaxy/entity/EntityManager.hpp"
#include "galaxy/events/Event.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/state/StateMachine.hpp"

namespace galaxy
{
	///
	/// State machine for managing scenes.
	///
	class SceneManager final : public StateMachine<Scene>
	{
	public:
		///
		/// Constructor.
		///
		SceneManager() noexcept;

		///
		/// Move constructor.
		///
		SceneManager(SceneManager&&);

		///
		/// Move assignment operator.
		///
		SceneManager& operator=(SceneManager&&);

		///
		/// Destructor.
		///
		virtual ~SceneManager();

		///
		/// Handle an event for a scene.
		///
		/// \param e Event that was triggered.
		///
		void on_event(SDL_Event& event);

		///
		/// Process events and updates.
		///
		/// \param em Entities to operate on.
		///
		void update(EntityManager& em);

		///
		/// Render scenes.
		///
		void render();

		///
		/// Removes all data.
		///
		void clear();

	private:
		///
		/// Copy constructor.
		///
		SceneManager(const SceneManager&) = delete;

		///
		/// Copy assignment operator.
		///
		SceneManager& operator=(const SceneManager&) = delete;
	};
} // namespace galaxy

/*
			///
			/// \brief Load app data file into scene manager.
			///
			/// Will remove any other loaded scenes and adjust the currently loaded scene.
			///
			/// \param appdata_file Zlib + Base64 encoded json data file. Should have .galaxy extension.
			///
			void load_app(const std::string& appdata_file);

			///
			/// Save all active scenes and sub data within those scenes.
			///
			/// \param file File to save data to.
			///
			void save_app(const std::string& file);

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
*/

#endif
