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
			using Map             = ankerl::unordered_dense::map<std::uint64_t, std::unique_ptr<Scene>>;
			using SystemContainer = meta::vector<std::unique_ptr<systems::System>>;

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
			/// Add a new scene.
			///
			/// \param name Scene name to assign.
			///
			/// \return Pointer to added scene.
			///
			[[maybe_unused]]
			Scene* add(const std::string& name);

			///
			/// Add a custom scene.
			///
			/// \tparam type Derived type of custom scene.
			///
			/// \param name Scene name to assign.
			///
			template<std::derived_from<Scene> Custom>
			[[maybe_unused]]
			Custom* add_custom(const std::string& name);

			///
			/// Get a specific scene.
			///
			/// \param name Name Id for scene.
			///
			/// \return Reference to requested scene.
			///
			[[nodiscard]]
			Scene* get(const std::string& name);

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
			[[nodiscard]]
			bool has(const std::string& name);

			///
			/// Scene to set to currently active.
			///
			/// \param name Scene name.
			///
			void set_scene(const std::string& name);

			///
			/// \brief Add a system to the manager.
			///
			/// \tparam System Type of system to create.
			/// \tparam Args Constructor arguments for system.
			///
			/// Systems will be updated in the order in which they are created.
			///
			/// \param args Constructor arguments for system.
			///
			template<meta::is_system System, typename... Args>
			void create_system(Args&&... args);

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
			/// Only update rendering.
			///
			void only_update_rendering();

			///
			/// Handle rendering.
			///
			void render();

			///
			/// Deletes all scene data.
			///
			void clear();

			///
			/// Get current scene.
			///
			/// \return Pointer to current scene, nullptr if no scene.
			///
			[[nodiscard]]
			scene::Scene* current() const;

			///
			/// Get all scenes.
			///
			/// \return Reference to scene object.
			///
			[[nodiscard]]
			const Map& map() const;

			///
			/// Are there any scenes.
			///
			/// \return True if no scenes.
			///
			[[nodiscard]]
			bool empty() const;

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
			///
			/// Scene map.
			///
			Map m_scenes;

			///
			/// Current scene.
			///
			scene::Scene* m_current;

			///
			/// Stores systems.
			///
			SystemContainer m_systems;

			///
			/// Rendersystem index.
			///
			std::size_t m_rendersystem_index;
		};

		template<std::derived_from<Scene> Custom>
		inline Custom* SceneManager::add_custom(const std::string& name)
		{
			const auto hash = math::fnv1a_64(name.c_str());

			if (!m_scenes.contains(hash))
			{
				m_scenes[hash] = std::make_unique<Custom>(name);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to add a duplicate scene '{0}'.", name);
			}

			return static_cast<Custom*>(m_scenes[hash].get());
		}

		template<meta::is_system System, typename... Args>
		inline void SceneManager::create_system(Args&&... args)
		{
			auto ptr = std::make_unique<System>(std::forward<Args>(args)...);
			m_systems.emplace_back(std::move(ptr));

			if constexpr (std::is_same<System, systems::RenderSystem>::value)
			{
				m_rendersystem_index = m_systems.size() - 1;
			}
		}
	} // namespace scene
} // namespace galaxy

#endif
