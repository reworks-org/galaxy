///
/// Rectangle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RECTANGLE_HPP_
#define GALAXY_GRAPHICS_RECTANGLE_HPP_

#include <raylib.hpp>

#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
GALAXY_DISABLE_WARNING(26437)
#endif

namespace galaxy
{
	namespace graphics
	{
		class Rectangle final : public ::Rectangle
		{
		public:
			///
			/// Constructor.
			///
			Rectangle() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X value.
			/// \param y Y value.
			/// \param width Width value.
			/// \param height Height value.
			///
			Rectangle(const float x, const float y, const float width, const float height) noexcept;

			///
			/// Raylib rectangle constructor.
			///
			/// \param rect Rectangle to construct from.
			///
			Rectangle(const ::Rectangle& rect) noexcept;

			///
			/// Copy constructor.
			///
			Rectangle(const Rectangle&) noexcept;

			///
			/// Move constructor.
			///
			Rectangle(Rectangle&&) noexcept;

			///
			/// Copy assignment operator.
			///
			[[maybe_unused]]
			Rectangle& operator=(const Rectangle&) noexcept;

			///
			/// Move assignment operator.
			///
			[[maybe_unused]]
			Rectangle& operator=(Rectangle&&) noexcept;

			///
			/// Destructor.
			///
			~Rectangle() noexcept;

			///
			/// Check if point is inside rectangle.
			///
			/// \param point The point to check.
			///
			/// \return True if the point is inside the rectangle, false otherwise.
			///
			[[nodiscard]]
			bool check_collision_point_rec(const Vector2& point) noexcept;

			///
			/// Get collision rectangle for two rectangles collision.
			///
			/// \param rec2 The second rectangle.
			///
			/// \return The collision rectangle.
			///
			[[nodiscard]]
			Rectangle get_collision_rec(const Rectangle& rec2) noexcept;

			///
			/// Get rectangle as a Vec4.
			///
			/// \return Vector 4 of rectangle.
			///
			[[nodiscard]]
			Vector4 to_vec4() noexcept;

		private:
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
