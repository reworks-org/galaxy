///
/// SceneManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENE_SCENEMANAGER_HPP_
#define GALAXY_SCENE_SCENEMANAGER_HPP_

#include "galaxy/entity/EntityManager.hpp"
// #include "galaxy/events/Event.hpp"
#include "galaxy/scene/Scene.hpp"
#include "galaxy/state/StateMachine.hpp"
#include "galaxy/meta/Serializable.hpp"

namespace galaxy
{
	///
	/// State machine for managing scenes.
	///
	class SceneManager final : public StateMachine<Scene>, public Serializable
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
		void update();

		///
		/// Render scenes.
		///
		void render();

		///
		/// Removes all data.
		///
		void clear();

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

	private:
		///
		/// Copy constructor.
		///
		SceneManager(const SceneManager&) = delete;

		///
		/// Copy assignment operator.
		///
		SceneManager& operator=(const SceneManager&) = delete;

	private:
		///
		/// Entities belonging to all scenes.
		///
		EntityManager m_entity_manager;
	};
} // namespace galaxy

#endif
