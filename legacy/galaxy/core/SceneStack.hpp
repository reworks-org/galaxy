///
/// SceneStack.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SCENESTACK_HPP_
#define GALAXY_CORE_SCENESTACK_HPP_

#include "galaxy/core/Scene2D.hpp"

namespace galaxy
{
	namespace core
	{
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
			/// Create a scene.
			///
			/// \param name Scene name ID.
			///
			/// \return Pointer to newly created scene.
			///
			[[maybe_unused]] Scene2D* create(std::string_view name);

			///
			/// Get the scene at the top of the stack.
			///
			/// \return Pointer to scene.
			///
			[[nodiscard]] Scene2D* top();

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
			/// Process scene stack.
			///
			void events();

			///
			/// Update scene stack.
			///
			void update();

			///
			/// Perform pre-rendering operations outside of opengl begin/end of rendering.
			///
			void pre_render();

			///
			/// Render all scenes.
			///
			void render();

			///
			/// Get a specific scene.
			///
			/// \param name Scene ID.
			///
			/// \return Pointer to scene.
			///
			[[nodiscard]] Scene2D* get(std::string_view name);

			///
			/// Get all scenes.
			///
			/// \return Unordered node map containing all stack scenes.
			///
			[[nodiscard]] robin_hood::unordered_node_map<std::string, std::unique_ptr<Scene2D>>& get_scenes() noexcept;

			///
			/// Get an array of all scene keys.
			///
			/// \return Vector of strings.
			///
			[[nodiscard]] std::vector<std::string> get_scene_keys() noexcept;

			///
			/// Get scene stack.
			///
			/// \return Array of pointers to scenes.
			///
			[[nodiscard]] std::vector<Scene2D*>& get_stack() noexcept;

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
			robin_hood::unordered_node_map<std::string, std::unique_ptr<Scene2D>> m_scenes;

			///
			/// Simulate a stack using a std::vector.
			///
			std::vector<Scene2D*> m_stack;
		};
	} // namespace core
} // namespace galaxy

#endif