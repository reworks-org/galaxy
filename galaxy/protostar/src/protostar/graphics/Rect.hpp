///
/// Rect.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RECT_HPP_
#define PROTOSTAR_RECT_HPP_

#include <compare>

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Represents a rectangle object.
	///
	template<is_arithmetic type>
	class Rect final
	{
	public:
		///
		/// Default constructor.
		///
		Rect() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x X value.
		/// \param y Y value.
		/// \param width Width value.
		/// \param height Height value.
		///
		Rect(const type x, const type y, const type width, const type height) noexcept;

		///
		/// Default destructor.
		///
		~Rect() noexcept = default;

		///
		/// Does the rectangle contain the point (x, y).
		///
		/// \param x X coordinate.
		/// \param y Y coordinate.
		///
		/// \return True if contains the point.
		///
		[[nodiscard]] bool contains(const type x, const type y) noexcept;

		///
		/// Does the rectangle contain another rectangle.
		///
		/// \param b Rectangle to check.
		///
		/// \return Returns true if the rectangle is completely inside, not on the edge.
		///
		[[nodiscard]] bool contains(const Rect<type>& b) noexcept;

		///
		/// \brief Do the rectangles a and b overlap.
		///
		/// Credits: https://stackoverflow.com/a/306379
		///
		/// \param b Second rectangle to test.
		///
		/// \return Returns true if there is an overlap.
		///
		[[nodiscard]] bool overlaps(const Rect<type>& b) noexcept;

		///
		/// Spaceship operator.
		///
		auto operator<=>(const Rect&) const = default;

		///
		/// X position.
		///
		type m_x;

		///
		/// Y position.
		///
		type m_y;

		///
		/// Width of rectangle.
		///
		type m_width;

		///
		/// Height of rectangle.
		///
		type m_height;

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
		[[nodiscard]] bool value_in_range(const type value, const type min, const type max) noexcept;
	};

	template<is_arithmetic type>
	inline Rect<type>::Rect() noexcept
	    : m_x {0}, m_y {0}, m_width {0}, m_height {0}
	{
	}

	template<is_arithmetic type>
	inline Rect<type>::Rect(const type x, const type y, const type width, const type height) noexcept
	    : m_x {x}, m_y {y}, m_width {width}, m_height {height}
	{
	}

	template<is_arithmetic type>
	inline bool Rect<type>::contains(const type x, const type y) noexcept
	{
		// Checks if the rectangle contains the point (x, y) using some basic math.
		return ((x > m_x) && (x < (m_x + m_width)) && (y > m_y) && (y < (m_y + m_height)));
	}

	template<is_arithmetic type>
	inline bool Rect<type>::contains(const Rect<type>& b) noexcept
	{
		// Checks if the rectangle contains another rectangle using math.
		return ((b.m_x + b.m_width) < (m_x + m_width) && (b.m_x) > (m_x) && (b.m_y) > (m_y) &&
			(b.m_y + b.m_height) < (m_y + m_height));
	}

	template<is_arithmetic type>
	inline bool Rect<type>::overlaps(const Rect<type>& b) noexcept
	{
		// Check for overlaps using math.
		bool x = value_in_range(m_x, b.m_x, b.m_x + b.m_width) || value_in_range(b.m_x, m_x, m_x + m_width);

		bool y =
		    value_in_range(m_y, b.m_y, b.m_y + b.m_height) || value_in_range(b.m_y, m_y, m_y + m_height);

		return x && y;
	}

	template<is_arithmetic type>
	inline bool Rect<type>::value_in_range(const type value, const type min, const type max) noexcept
	{
		// Check if a value is between min and max - i.e. in range.
		return (value >= min) && (value <= max);
	}
} // namespace pr

#endif