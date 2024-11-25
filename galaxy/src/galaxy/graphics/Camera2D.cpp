///
/// Camera2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Camera2D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera2D::Camera2D() noexcept
		{
			offset   = {0.0f, 0.0f};
			target   = {0.0f, 0.0f};
			rotation = 0.0f;
			zoom     = 1.0f;
		}

		Camera2D::~Camera2D() noexcept
		{
		}

		math::Vector2 Camera2D::get_world_to_screen(const math::Vector2& position) noexcept
		{
			return {GetWorldToScreen2D(position, *this)};
		}

		math::Vector2 Camera2D::get_screen_to_world(const math::Vector2& position) noexcept
		{
			return {GetScreenToWorld2D(position, *this)};
		}

		math::Matrix Camera2D::get_camera_matrix() noexcept
		{
			return {GetCameraMatrix2D(*this)};
		}
	} // namespace graphics
} // namespace galaxy
