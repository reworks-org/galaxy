///
/// SceneManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENE_SCENEMANAGER_HPP_
#define GALAXY_SCENE_SCENEMANAGER_HPP_

#include "galaxy/scene/Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// Scene saving/loading, pushing, popping, creating.
		/// Also manages entities.
		///
		class SceneManager final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			SceneManager() noexcept;

			///
			/// Destructor.
			///
			virtual ~SceneManager() noexcept;

			///
			/// Create scene in engine.
			///
			/// \param scene Scene name.
			///
			/// \return Shared pointer to created scene, or nullptr on failure.
			///
			[[maybe_unused]]
			std::shared_ptr<Scene> create(const std::string& scene);

			///
			/// Clears all from stack and sets as active.
			///
			/// \param scene Scene name.
			///
			void set(const std::string& scene);

			///
			/// Push a scene onto the top of stack.
			///
			/// \param scene Scene name.
			///
			void push(const std::string& scene);

			///
			/// Remove scene on top of stack.
			///
			void pop();

			///
			/// Pop all scenes.
			///
			void pop_all();

			///
			/// Get top scene.
			///
			[[nodiscard]]
			std::shared_ptr<Scene> top() noexcept;

			///
			/// Process events and updates.
			///
			void update();

			///
			/// Render scenes.
			///
			void render();

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
			/// Deletes all scene data.
			///
			void clear();

			///
			/// Are there any scenes.
			///
			/// \return True if no scenes.
			///
			[[nodiscard]]
			bool empty() const noexcept;

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
			/// Scene storage.
			///
			ankerl::unordered_dense::map<std::uint64_t, std::shared_ptr<Scene>> m_scenes;

			///
			/// Active scenes.
			///
			std::vector<std::shared_ptr<Scene>> m_stack;

			///
			/// Entity data.
			///
			core::Registry m_registry;
		};
	} // namespace scene
} // namespace galaxy

#endif
