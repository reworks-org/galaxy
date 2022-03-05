///
/// SceneManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENEMANAGER_HPP_
#define GALAXY_STATE_SCENEMANAGER_HPP_

#include "galaxy/state/Scene.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// Typedef for scene container.
		///
		using SceneContainer = robin_hood::unordered_flat_map<std::string, std::shared_ptr<Scene>>;

		///
		/// Scene saving/loading, creation, changing and loading.
		///
		class SceneManager final
		{
		public:
			///
			/// Constructor.
			///
			SceneManager() noexcept;

			///
			/// Destructor.
			///
			~SceneManager() noexcept;

			///
			/// Create a new scene.
			///
			/// \param name Scene name to assign.
			///
			/// \return Shared pointer to created scene.
			///
			[[maybe_unused]] std::shared_ptr<Scene> make(const std::string& name);

			///
			/// Change the current scene.
			///
			/// \param name Name id for scene.
			///
			void change(const std::string& name);

			///
			/// Get currently active scene.
			///
			/// \return Shared pointer to currently active scene.
			///
			[[nodiscard]] std::shared_ptr<Scene> current() noexcept;

			///
			/// Gets a specific scene.
			///
			/// \param name Name id for scene.
			///
			/// \return Shared pointer to requested scene.
			///
			[[nodiscard]] std::shared_ptr<Scene> get(const std::string& name) noexcept;

			///
			/// Save all active scenes and sub data within those scenes.
			///
			/// \param file File to save data to.
			///
			void save(std::string_view file);

			///
			/// Load data from file into scenes.
			///
			/// \param file File to load data from.
			///
			void load(std::string_view file);

			///
			/// \brief Remove all scenes.
			///
			/// Destroys scene data, so beware of pointers becoming invalid.
			///
			void clear();

		private:
			///
			/// Fast hashmap storing data.
			///
			SceneContainer m_scenes;

			///
			/// Currently active scene.
			///
			std::shared_ptr<Scene> m_current;
		};
	} // namespace state
} // namespace galaxy

#endif