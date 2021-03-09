///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SCENE_HPP_
#define GALAXY_CORE_SCENE_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Camera.hpp"

namespace galaxy
{
	namespace fs
	{
		class Serializer;
	} // namespace fs

	namespace core
	{
		///
		/// Represents a scene within the layer.
		///
		class Scene
		{
			friend class fs::Serializer;

		public:
			///
			/// Name constructor.
			///
			Scene(std::string_view name) noexcept;

			///
			/// Destructor.
			///
			virtual ~Scene() noexcept = default;

			///
			/// Process events.
			///
			virtual void events() = 0;

			///
			/// Process update (fixed timestep) logic.
			///
			/// \param dt Delta-Time from fixed timestep gameloop.
			///
			virtual void update(const double dt) = 0;

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			virtual void pre_render() = 0;

			///
			/// Render to screen.
			///
			virtual void render() = 0;

			///
			/// Get scene name.
			///
			/// \return Reference to a const std::string.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get scene world.
			///
			/// \return Reference to internal scene world.
			///
			[[nodiscard]] World& world() noexcept;

			///
			/// Get scene camera.
			///
			/// \return Reference to internal scene camera.
			///
			[[nodiscard]] graphics::Camera& camera() noexcept;

		private:
			///
			/// Constructor.
			///
			Scene() = delete;

		protected:
			///
			/// Scene name.
			///
			std::string m_name;

			///
			/// Entity/System manager.
			///
			World m_world;

			///
			/// Camera.
			///
			graphics::Camera m_camera;
		};
	} // namespace core

	namespace meta
	{
		template<typename Type>
		concept is_scene = std::derived_from<Type, core::Scene>;
	}
} // namespace galaxy

#endif