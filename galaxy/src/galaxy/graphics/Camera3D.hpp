///
/// Camera3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA3D_HPP_
#define GALAXY_GRAPHICS_CAMERA3D_HPP_

#include "galaxy/math/Vector2.hpp"
#include "galaxy/math/Matrix.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
#endif

namespace galaxy
{
	namespace graphics
	{
		///
		/// 3D camera for drawing with.
		///
		class Camera3D : public ::Camera3D
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param proj 3D camera projection.
			/// \param fov Camera Field of view.
			///
			Camera3D(const ray::CameraProjection proj, const float fov = 90.0f);

			///
			/// Argument constructor.
			///
			/// \param camera Raylib camera.
			///
			Camera3D(const ::Camera3D& camera) noexcept;

			///
			/// Destructor.
			///
			~Camera3D() noexcept;

			///
			/// Set from raylib camera3d.
			///
			/// \param camera Raylib camera.
			///
			void set(const ::Camera3D& camera) noexcept;

			///
			/// Get the screen space position for a 3D world space position.
			///
			/// \param position The world space position.
			///
			/// \return The screen space position.
			///
			[[nodiscard]]
			math::Vector2 get_world_to_screen(const math::Vector3& position) noexcept;

			///
			/// Get screen space position for a 3D world space position in a viewport.
			///
			/// \param position The world space position.
			/// \param width The viewport width.
			/// \param height The viewport height.
			///
			/// \return The screen space position.
			///
			[[nodiscard]]
			math::Vector2 get_world_to_screen_ex(const math::Vector3& position, const int width, const int height) noexcept;

			///
			/// Get a ray trace from screen position (i.e., mouse).
			///
			/// \param position The screen position.
			/// \param camera The camera.
			/// \return The ray trace.
			///
			//[[nodiscard]]
			// Ray get_screen_to_world_ray(const Vector2& position, const Camera& camera) noexcept;

			///
			/// Get a ray trace from screen position (i.e., mouse) in a viewport.
			///
			/// \param position The screen position.
			/// \param camera The camera.
			/// \param width The viewport width.
			/// \param height The viewport height.
			/// \return The ray trace.
			///
			//[[nodiscard]]
			// Ray get_screen_to_world_ray_ex(const Vector2& position, const Camera& camera, const int width, const int height) noexcept;

			///
			/// Get camera transform matrix (view matrix).
			///
			/// \return The camera transform matrix.
			///
			[[nodiscard]]
			math::Matrix get_camera_matrix() noexcept;

			///
			/// Update camera position for selected mode.
			///
			/// \param mode The update mode.
			///
			void update_camera(const ray::CameraMode mode) noexcept;

			///
			/// Update camera movement and rotation.
			///
			/// \param movement The movement vector.
			/// \param rotation The rotation vector.
			/// \param zoom The zoom factor.
			///
			void update_camera_pro(const math::Vector3& movement, const math::Vector3& rotation, const float zoom) noexcept;

			///
			/// Set Field of View.
			///
			/// \param fov Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic.
			///
			void set_fov(const float fov) noexcept;

			///
			/// Camera projection.
			///
			/// \param proj CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC.
			///
			void set_proj(const ray::CameraProjection proj) noexcept;

		private:
			///
			/// Constructor.
			///
			Camera3D() = delete;
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
