///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/state/Layers.hpp"

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
			/// Process scene events.
			///
			void events();

			///
			/// Process updates.
			///
			void update();

			///
			/// Render scene.
			///
			void render();

			///
			/// Only update the rendersystem.
			///
			void update_rendersystem();

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
			/// \return Reference to layers stack.
			///
			[[nodiscard]] Layers& layers() noexcept;

			///
			/// Get camera.
			///
			/// \return Reference to camera.
			///
			[[nodiscard]] graphics::Camera& get_camera() noexcept;

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
			Layers m_layer_stack;

			///
			/// Camera.
			///
			graphics::Camera m_camera;
		};
	} // namespace state
} // namespace galaxy

#endif