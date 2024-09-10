///
/// Rect.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_RECT_HPP_
#define GALAXY_MATH_RECT_HPP_

#include <compare>

#include <glm/vec2.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// Represents a rectangle object.
		///
		/// \tparam Type Arithmetic type to use for a rectangle.
		///
		template<meta::is_arithmetic Type>
		class Rect final
		{
		public:
			///
			/// Constructor.
			///
			Rect();

			///
			/// Argument constructor.
			///
			/// \param x X value.
			/// \param y Y value.
			/// \param width Width value.
			/// \param height Height value.
			///
			Rect(const Type x, const Type y, const Type width, const Type height);

			///
			/// Destructor.
			///
			~Rect();

			///
			/// Does the rectangle contain the point (x, y).
			///
			/// \param _x X coordinate.
			/// \param _y Y coordinate.
			///
			/// \return True if contains the point.
			///
			[[nodiscard]]
			bool contains(const Type _x, const Type _y) const;

			///
			/// Does the rectangle contain another rectangle.
			///
			/// \param b Rectangle to check.
			///
			/// \return Returns true if the rectangle is completely inside, not on the edge.
			///
			[[nodiscard]]
			bool contains(const Rect<Type>& b) const;

			///
			/// Do the rectangles a and b overlap.
			///
			/// \param b Second rectangle to test.
			///
			/// \return Returns true if there is an overlap.
			///
			[[nodiscard]]
			bool overlaps(const Rect<Type>& b);

			///
			/// Do two rectangles intersect.
			///
			/// \param b Rectangle to test.
			///
			/// \return True if the rectangles intersect.
			///
			[[nodiscard]]
			bool intersects(const Rect<Type>& b) const;

			///
			/// Set the top-left position.
			///
			/// \param pos {x, y} coordinate.
			///
			void set_top_left(const glm::vec2& pos);

			///
			/// Get top right corner.
			///
			/// \return x + width.
			///
			[[nodiscard]]
			Type get_right() const;

			///
			/// Get bottom left corner.
			///
			/// \return y + height.
			///
			[[nodiscard]]
			Type get_bottom() const;

			///
			/// Get the upper-left coordinate.
			///
			/// \return {x, y}.
			///
			[[nodiscard]]
			glm::vec<2, Type, glm::defaultp> get_top_left() const;

			///
			/// Gets the center of the rectangle.
			///
			/// \return Center point of rectangle.
			///
			[[nodiscard]]
			glm::vec<2, Type, glm::defaultp> get_center() const;

			///
			/// Gets width and height of rectangle.
			///
			/// \return {width, height}.
			///
			[[nodiscard]]
			glm::vec<2, Type, glm::defaultp> get_size() const;

			///
			/// Comparison operator.
			///
			[[nodiscard]]
			auto operator<=>(const Rect&) const = default;

		public:
			///
			/// X position.
			///
			Type x;

			///
			/// Y position.
			///
			Type y;

			///
			/// Width of rectangle.
			///
			Type width;

			///
			/// Height of rectangle.
			///
			Type height;

		private:
			///
			/// Private function to determine if value is in range. Inclusive.
			/// Credits: https://stackoverflow.com/a/306379
			///
			/// \param value Value to check if in the min-max range.
			/// \param min Lower range.
			/// \param max Upper range.
			///
			/// \return True if value is inbetween min and max. Inclusive.
			///
			[[nodiscard]]
			bool value_in_range(const Type value, const Type min, const Type max);
		};

		///
		/// Type definition for an int rectangle.
		///
		typedef Rect<int> iRect;

		///
		/// Type definition for a floating point rectangle.
		///
		typedef Rect<float> fRect;

		template<meta::is_arithmetic Type>
		inline Rect<Type>::Rect()
			: x {0}
			, y {0}
			, width {0}
			, height {0}
		{
		}

		template<meta::is_arithmetic Type>
		inline Rect<Type>::Rect(const Type x, const Type y, const Type width, const Type height)
			: x {x}
			, y {y}
			, width {width}
			, height {height}
		{
		}

		template<meta::is_arithmetic Type>
		inline Rect<Type>::~Rect()
		{
		}

		template<meta::is_arithmetic Type>
		inline bool Rect<Type>::contains(const Type _x, const Type _y) const
		{
			// Checks if the rectangle contains the point (x, y) using some basic math.
			return ((_x > x) && (_x < (x + width)) && (_y > y) && (_y < (y + height)));
		}

		template<meta::is_arithmetic Type>
		inline bool Rect<Type>::contains(const Rect<Type>& b) const
		{
			return x <= b.x && b.get_right() <= get_right() && y <= b.y && b.get_bottom() <= get_bottom();
		}

		template<meta::is_arithmetic Type>
		inline bool Rect<Type>::overlaps(const Rect<Type>& b)
		{
			// Check for overlaps using math.
			const auto _x = value_in_range(x, b.x, b.x + b.width) || value_in_range(b.x, x, x + width);
			const auto _y = value_in_range(y, b.y, b.y + b.height) || value_in_range(b.y, y, y + height);

			return _x && _y;
		}

		template<meta::is_arithmetic Type>
		inline bool Rect<Type>::intersects(const Rect<Type>& b) const
		{
			return !(x >= b.get_right() || get_right() <= b.x || y >= b.get_bottom() || get_bottom() <= b.y);
		}

		template<meta::is_arithmetic Type>
		inline void Rect<Type>::set_top_left(const glm::vec2& pos)
		{
			x = pos.x;
			y = pos.y;
		}

		template<meta::is_arithmetic Type>
		inline Type Rect<Type>::get_right() const
		{
			return x + width;
		}

		template<meta::is_arithmetic Type>
		inline Type Rect<Type>::get_bottom() const
		{
			return y + height;
		}

		template<meta::is_arithmetic Type>
		inline glm::vec<2, Type, glm::defaultp> Rect<Type>::get_top_left() const
		{
			return {x, y};
		}

		template<meta::is_arithmetic Type>
		inline glm::vec<2, Type, glm::defaultp> Rect<Type>::get_center() const
		{
			return {x + width / 2.0, y + height / 2.0};
		}

		template<meta::is_arithmetic Type>
		inline glm::vec<2, Type, glm::defaultp> Rect<Type>::get_size() const
		{
			return {width, height};
		}

		template<meta::is_arithmetic Type>
		inline bool Rect<Type>::value_in_range(const Type value, const Type min, const Type max)
		{
			// Check if a value is between min and max - i.e. in range.
			return (value >= min) && (value <= max);
		}
	} // namespace math
} // namespace galaxy

#endif
