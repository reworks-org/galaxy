///
/// SceneManager.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENEMANAGER_HPP_
#define GALAXY_STATE_SCENEMANAGER_HPP_

#include "galaxy/math/FNV1a.hpp"
#include "galaxy/scene/Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// Scene saving/loading, creation, changing and loading.
		///
		class SceneManager final : public fs::Serializable
		{
			using Map  = robin_hood::unordered_flat_map<std::uint64_t, std::shared_ptr<Scene>>;
			using List = meta::vector<std::shared_ptr<Scene>>;

		  public:
			///
			/// Constructor.
			///
			SceneManager();

			///
			/// Destructor.
			///
			virtual ~SceneManager();

			///
			/// Add a new scene.
			///
			/// \param name Scene name to assign.
			///
			/// \return Pointer to added scene.
			///
			[[maybe_unused]] std::shared_ptr<Scene> add(const std::string& name);

			///
			/// Add a custom scene.
			///
			/// \tparam type Derived type of custom scene.
			///
			/// \param name Scene name to assign.
			///
			/// \return Pointer to added scene.
			///
			template<typename T>
			[[maybe_unused]] std::shared_ptr<T> add_custom(const std::string& name);

			///
			/// Get a specific scene.
			///
			/// \param name Name Id for scene.
			///
			/// \return Reference to requested scene.
			///
			[[nodiscard]] std::shared_ptr<Scene> get(const std::string& name);

			///
			/// \brief Remove a specific scene.
			///
			/// Preserves insertion order.
			///
			/// \param name Id for scene.
			///
			void remove(const std::string& name);

			///
			/// Does a scene exist.
			///
			/// \param name Id for scene.
			///
			/// \return True if exists.
			///
			[[nodiscard]] bool has(const std::string& name);

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
			/// Handle events and update logic.
			///
			void update();

			///
			/// Handle rendering.
			///
			void render();

			///
			/// Only update ui.
			///
			void update_ui();

			///
			/// Only update rendering.
			///
			void update_rendering();

			///
			/// Deletes all scene data.
			///
			void clear();

			///
			/// Get all scenes.
			///
			/// \return Reference to scene object.
			///
			[[nodiscard]] const Map& map() const;

			///
			/// Get scene order.
			///
			/// \return Reference to scene object.
			///
			[[nodiscard]] const List& list() const;

			///
			/// Get number of scenes.
			///
			/// \return Number of scenes.
			///
			[[nodiscard]] std::size_t size() const;

			///
			/// Are there any scenes.
			///
			/// \return True if no scenes.
			///
			[[nodiscard]] bool empty() const;

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
			/// Scene map.
			///
			Map m_scenes;

			///
			/// Scene execution order.
			///
			List m_order;
		};

		template<typename T>
		inline std::shared_ptr<T> SceneManager::add_custom(const std::string& name)
		{
			const auto hash = math::fnv1a_64(name.c_str());

			if (!m_scenes.contains(hash))
			{
				auto child = std::make_shared<T>(name);

				m_order.push_back(child);
				m_scenes[hash] = std::static_pointer_cast<Scene>(child);

				return child;
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to add a duplicate scene '{0}'.", name);
				return std::static_pointer_cast<T>(m_scenes[hash]);
			}
		}
	} // namespace scene
} // namespace galaxy

#endif
