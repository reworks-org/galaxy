///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include "galaxy/resource/Resources.hpp"
#include "galaxy/state/LayerStack.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// Represents a scene in a game. Like the menu, game, etc.
		///	Does not share resources.
		///
		class Scene final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Scene() noexcept;

			///
			/// Destructor.
			///
			virtual ~Scene() noexcept;

			///
			/// \brief Load data required by this scene.
			///
			/// Displays a loading screen overriding whatever is currently rendering.
			///
			void load();

			///
			/// Unload scene data.
			///
			void unload();

			///
			/// Process Scene events.
			///
			void events();

			///
			/// Process update (fixed timestep) logic.
			///
			void update();

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			void pre_render();

			///
			/// Render Scene.
			///
			void render();

			///
			/// Set Scene name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(const std::string& name) noexcept;

			///
			/// Get Scene name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get layer stack.
			///
			/// \return Reference to scene stack.
			///
			[[nodiscard]] LayerStack& get_stack() noexcept;

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
			/// Scene name for debug purposes.
			///
			std::string m_name;

			///
			/// Controls current application layer.
			///
			LayerStack m_layer_stack;

			///
			/// Resources used by scene.
			///
			resource::Resources m_resources;

			///
			/// Backup of deserialization JSON.
			///
			nlohmann::json m_deserialization_json;
		};
	} // namespace state
} // namespace galaxy

#endif