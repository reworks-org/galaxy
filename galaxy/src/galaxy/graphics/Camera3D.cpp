///
/// Camera3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Camera3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera3D::Camera3D(const ray::CameraProjection proj, const float fov)
		{
			position   = {0.0f, 0.0f, 0.0f};
			target     = {0.0f, 0.0f, 0.0f};
			up         = {0.0f, 0.0f, 0.0f};
			fovy       = fov;
			projection = proj;
		}

		Camera3D::Camera3D(const ::Camera3D& camera) noexcept
		{
			set(camera);
		}

		Camera3D::~Camera3D() noexcept
		{
		}

		void Camera3D::set(const ::Camera3D& camera) noexcept
		{
			position   = camera.position;
			target     = camera.target;
			up         = camera.up;
			fovy       = camera.fovy;
			projection = camera.projection;
		}

		math::Vector2 Camera3D::get_world_to_screen(const math::Vector3& position) noexcept
		{
			return {GetWorldToScreen(position, *this)};
		}

		math::Vector2 Camera3D::get_world_to_screen_ex(const math::Vector3& position, const int width, const int height) noexcept
		{
			return {GetWorldToScreenEx(position, *this, width, height)};
		}

		math::Matrix Camera3D::get_camera_matrix() noexcept
		{
			return {GetCameraMatrix(*this)};
		}

		void Camera3D::update_camera(const ray::CameraMode mode) noexcept
		{
			UpdateCamera(this, static_cast<int>(mode));
		}

		void Camera3D::update_camera_pro(const math::Vector3& movement, const math::Vector3& rotation, const float zoom) noexcept
		{
			UpdateCameraPro(this, movement, rotation, zoom);
		}

		void Camera3D::set_fov(const float fov) noexcept
		{
			fovy = fov;
		}

		void Camera3D::set_proj(const ray::CameraProjection proj) noexcept
		{
			projection = proj;
		}
	} // namespace graphics
} // namespace galaxy
