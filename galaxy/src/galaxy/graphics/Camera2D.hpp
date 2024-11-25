///
/// Camera2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CAMERA2D_HPP_
#define GALAXY_GRAPHICS_CAMERA2D_HPP_

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
		/// 2D camera for drawing with.
		///
		class Camera2D : public ::Camera2D
		{
		public:
			///
			/// Constructor.
			///
			Camera2D() noexcept;

			///
			/// Argument constructor.
			///
			/// \param camera Raylib camera.
			///
			Camera2D(const ::Camera2D& camera) noexcept;

			///
			/// Destructor.
			///
			~Camera2D() noexcept;

			///
			/// Set from raylib camera2d.
			///
			/// \param camera Raylib camera.
			///
			void set(const ::Camera2D& camera) noexcept;

			///
			/// Get the screen space position for a 2D camera world space position.
			///
			/// \param position The world space position.
			///
			/// \return The screen space position.
			///
			[[nodiscard]]
			math::Vector2 get_world_to_screen(const math::Vector2& position) noexcept;

			///
			/// Get the world space position for a 2D camera screen space position.
			///
			/// \param position The screen space position.
			///
			/// \return The world space position.
			///
			[[nodiscard]]
			math::Vector2 get_screen_to_world(const math::Vector2& position) noexcept;

			///
			/// Get camera transform matrix (view matrix).
			///
			/// \return The camera transform matrix.
			///
			[[nodiscard]]
			math::Matrix get_camera_matrix() noexcept;
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
