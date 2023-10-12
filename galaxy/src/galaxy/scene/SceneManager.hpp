///
/// SceneManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENEMANAGER_HPP_
#define GALAXY_STATE_SCENEMANAGER_HPP_

#include "galaxy/scene/Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// Typedef for scene container.
		///
		using SceneContainer = robin_hood::unordered_flat_map<std::string, std::shared_ptr<Scene>>;

		///
		/// Scene saving/loading, creation, changing and loading.
		///
		class SceneManager final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			SceneManager();

			///
			/// Destructor.
			///
			~SceneManager();

			///
			/// Create a new scene.
			///
			/// \param name Scene name to assign.
			///
			/// \return Shared pointer to created scene.
			///
			[[maybe_unused]] std::weak_ptr<Scene> make_scene(const std::string& name);

			///
			/// \brief Add an already existing scene.
			///
			/// I.e. if you have a custom scene.
			///
			/// \param name Scene identifier.
			/// \param scene Pointer to scene object. Let the manager take ownership.
			///
			void add_existing_scene(const std::string& name, std::shared_ptr<Scene> scene);

			///
			/// Set the currently active scene.
			///
			/// \param name Name id for scene.
			///
			void set_scene(const std::string& name);

			///
			/// Load data from memory into scenes.
			///
			/// \param data JSON data to parse.
			///
			void load_scene(const std::string& data);

			///
			/// Unloads the currently active scene.
			///
			void unload_scene();

			///
			/// \brief Load app data file into scene manager.
			///
			/// Will remove any other loaded scenes and adjust the currently loaded scene.
			///
			/// \param appdata_file Zlib + Base64 encoded json data file. Should have .galaxy extension.
			///
			void load_appdata(std::string_view appdata_file);

			///
			/// Save all active scenes and sub data within those scenes.
			///
			/// \param file File to save data to.
			///
			void save_appdata(std::string_view file);

			///
			/// Gets a specific scene.
			///
			/// \param name Name id for scene.
			///
			/// \return Shared pointer to requested scene.
			///
			[[nodiscard]] std::weak_ptr<Scene> get(const std::string& name);

			///
			/// \brief Remove a specific scene.
			///
			/// You cant remove the currently active scene.
			///
			/// \param name Id for scene.
			///
			/// \return True if scene removed, false if not, because scene is active.
			///
			[[nodiscard]] bool remove(const std::string& name);

			///
			/// \brief Destroys all scenes.
			///
			/// Beware of pointers becoming invalid.
			///
			/// \param clear_current Also clear the currently active scene.
			///
			void clear(bool clear_current = false);

			///
			/// Get currently active scene.
			///
			/// \return Ref to currently active scene.
			///
			[[nodiscard]] Scene& current();

			///
			/// Checks if there is a currently active scene.
			///
			/// \return True if there is a valid current scene.
			///
			[[nodiscard]] bool has_current() const;

			///
			/// Get all scenes.
			///
			/// \return SceneContainer reference.
			///
			[[nodiscard]] SceneContainer& all();

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
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
			/// Fast hashmap storing data.
			///
			SceneContainer m_scenes;

			///
			/// Currently active scene.
			///
			std::shared_ptr<Scene> m_current;
		};
	} // namespace scene
} // namespace galaxy

#endif