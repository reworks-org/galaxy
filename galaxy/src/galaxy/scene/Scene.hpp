///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/core/TiledMap.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/scene/Layers.hpp"

namespace galaxy
{
	namespace scene
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
			void load() noexcept;

			///
			/// Unload scene data.
			///
			void unload() noexcept;

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

		public:
			///
			/// Camera.
			///
			graphics::Camera m_camera;

			///
			/// Entitys, events, etc.
			///
			core::World m_world;

			///
			/// Manages the map for this layer.
			///
			core::TiledMap m_map;

		private:
			///
			/// Scene name for debug purposes.
			///
			std::string m_name;

			///
			/// Controls current application layer.
			///
			Layers m_layer_stack;
		};
	} // namespace scene
} // namespace galaxy

#endif