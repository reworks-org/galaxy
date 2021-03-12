///
/// SceneStack.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SCENESTACK_HPP_
#define GALAXY_CORE_SCENESTACK_HPP_

#include <memory>
#include <vector>

#include <robin_hood.h>

#include "galaxy/meta/Concepts.hpp"
#include "galaxy/scenes/GUIScene.hpp"
#include "galaxy/scenes/WorldScene.hpp"

namespace galaxy
{
	namespace scenes
	{
		///
		/// Shorthand.
		///
		using SceneStorage = robin_hood::unordered_node_map<std::string, std::shared_ptr<scenes::Scene>>;

		///
		/// Restrict template types to only scenes.
		///
		template<typename Type>
		concept is_scene = std::derived_from<Type, scenes::Scene>;

		///
		/// Stack based storage for scenes.
		///
		class SceneStack final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			SceneStack() noexcept;

			///
			/// Destructor.
			///
			~SceneStack();

			///
			/// Constructs a scene.
			///
			/// \param name Name of the scene.
			///
			/// \return Shared pointer to created scene. nullptr if failed.
			///
			template<is_scene Scene>
			[[maybe_unused]] std::shared_ptr<Scene> create(std::string_view name);

			///
			/// Retrieve an scene from the provided name.
			///
			/// \param name Name of the scene.
			///
			/// \return Pointer to scene.
			///
			template<is_scene Scene>
			[[nodiscard]] std::shared_ptr<Scene> get(std::string_view name);

			///
			/// Get the scene on top of the stack.
			///
			/// \return Returns pointer to topmost scene.
			///
			[[nodiscard]] std::shared_ptr<scenes::Scene> top();

			///
			/// Get the scene on top of the stack.
			///
			/// \return Returns pointer to topmost scene.
			///
			template<is_scene Scene>
			[[nodiscard]] std::shared_ptr<Scene> top();

			///
			/// Process all scene events.
			///
			void events();

			///
			/// Update scenes.
			///
			/// \param dt Delta time from gameloop.
			///
			void update(const double dt);

			///
			/// Perform pre-rendering operations outside of opengl begin/end of rendering.
			///
			void pre_render();

			///
			/// Render all scenes.
			///
			void render();

			///
			/// Push a scene based on name.
			///
			/// \param name Name of the scene to push.
			///
			void push(std::string_view name);

			///
			/// Destroy top-most scene.
			///
			void pop();

			///
			/// Destroy all scenes.
			///
			void clear();

			///
			/// Get all scenes.
			///
			/// \return Reference to scenestorage (unordered_map).
			///
			[[nodiscard]] SceneStorage& get_scenes() noexcept;

			///
			/// Get an array of all scene keys.
			///
			/// \return Vector of strings.
			///
			[[nodiscard]] std::vector<std::string> get_scene_keys() noexcept;

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
			/// Copy constructor.
			///
			SceneStack(const SceneStack&) = delete;

			///
			/// Move constructor.
			///
			SceneStack(SceneStack&&) = delete;

			///
			/// Copy assignment operator.
			///
			SceneStack& operator=(const SceneStack&) = delete;

			///
			/// Move assignment operator.
			///
			SceneStack& operator=(SceneStack&&) = delete;

		private:
			///
			/// Holds scene data.
			///
			SceneStorage m_scenes;

			///
			/// Simulate a stack using a std::vector.
			///
			std::vector<std::shared_ptr<scenes::Scene>> m_stack;
		};

		template<is_scene Scene>
		inline std::shared_ptr<Scene> SceneStack::create(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_scenes.contains(str))
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to create a scene that already exists!");
			}
			else
			{
				m_scenes[str] = std::make_shared<Scene>(str);
				return std::static_pointer_cast<Scene>(m_scenes[str]);
			}
		}

		template<is_scene Scene>
		inline std::shared_ptr<Scene> SceneStack::top()
		{
			if (!m_stack.empty())
			{
				return std::static_pointer_cast<Scene>(m_stack.back());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No scenes in stack!");
				return nullptr;
			}
		}

		template<is_scene Scene>
		inline std::shared_ptr<Scene> SceneStack::get(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_scenes.contains(str))
			{
				return std::static_pointer_cast<Scene>(m_scenes[str]);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to retrieve non-existant scene: {0}.", name);
				return nullptr;
			}
		}
	} // namespace scenes
} // namespace galaxy

#endif