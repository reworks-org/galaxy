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
		Rect() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x X value.
		/// \param y Y value.
		/// \param width Width value.
		/// \param height Height value.
		///
		Rect(const Type x, const Type y, const Type width, const Type height) noexcept;

		///
		/// Destructor.
		///
		~Rect() noexcept;

		///
		/// Does the rectangle contain the point (x, y).
		///
		/// \param _x X coordinate.
		/// \param _y Y coordinate.
		///
		/// \return True if contains the point.
		///
		[[nodiscard]]
		bool contains(const Type _x, const Type _y) const noexcept;

		///
		/// Does the rectangle contain another rectangle.
		///
		/// \param b Rectangle to check.
		///
		/// \return Returns true if the rectangle is completely inside, not on the edge.
		///
		[[nodiscard]]
		bool contains(const Rect<Type>& b) const noexcept;

		///
		/// Do the rectangles a and b overlap.
		///
		/// \param b Second rectangle to test.
		///
		/// \return Returns true if there is an overlap.
		///
		[[nodiscard]]
		bool overlaps(const Rect<Type>& b) noexcept;

		///
		/// Do two rectangles intersect.
		///
		/// \param b Rectangle to test.
		///
		/// \return True if the rectangles intersect.
		///
		[[nodiscard]]
		bool intersects(const Rect<Type>& b) const noexcept;

		///
		/// Set the top-left position.
		///
		/// \param pos {x, y} coordinate.
		///
		void set_top_left(const glm::vec<2, Type, glm::defaultp>& pos) noexcept;

		///
		/// Get top right corner.
		///
		/// \return x + width.
		///
		[[nodiscard]]
		Type get_right() const noexcept;

		///
		/// Get bottom left corner.
		///
		/// \return y + height.
		///
		[[nodiscard]]
		Type get_bottom() const noexcept;

		///
		/// Get the upper-left coordinate.
		///
		/// \return {x, y}.
		///
		[[nodiscard]]
		glm::vec<2, Type, glm::defaultp> get_top_left() const noexcept;

		///
		/// Gets the center of the rectangle.
		///
		/// \return Center point of rectangle.
		///
		[[nodiscard]]
		glm::vec<2, Type, glm::defaultp> get_center() const noexcept;

		///
		/// Gets width and height of rectangle.
		///
		/// \return {width, height}.
		///
		[[nodiscard]]
		glm::vec<2, Type, glm::defaultp> get_size() const noexcept;

		///
		/// Comparison operator.
		///
		[[nodiscard]]
		auto operator<=>(const Rect&) const noexcept = default;

	public:
		///
		/// X position.
		///
		Type m_xpos;

		///
		/// Y position.
		///
		Type m_ypos;

		///
		/// Width of rectangle.
		///
		Type m_width;

		///
		/// Height of rectangle.
		///
		Type m_height;

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
		bool value_in_range(const Type value, const Type min, const Type max) noexcept;
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
	inline Rect<Type>::Rect() noexcept
		: m_xpos {0}
		, m_ypos {0}
		, m_width {0}
		, m_height {0}
	{
	}

	template<meta::is_arithmetic Type>
	inline Rect<Type>::Rect(const Type x, const Type y, const Type width, const Type height) noexcept
		: m_xpos {x}
		, m_ypos {y}
		, m_width {width}
		, m_height {height}
	{
	}

	template<meta::is_arithmetic Type>
	inline Rect<Type>::~Rect() noexcept
	{
	}

	template<meta::is_arithmetic Type>
	inline bool Rect<Type>::contains(const Type x, const Type y) const noexcept
	{
		// Checks if the rectangle contains the point (x, y) using some basic math.
		return ((x > m_xpos) && (x < (m_xpos + m_width)) && (y > m_ypos) && (y < (m_ypos + m_height)));
	}

	template<meta::is_arithmetic Type>
	inline bool Rect<Type>::contains(const Rect<Type>& b) const noexcept
	{
		return m_xpos <= b.m_xpos && b.get_right() <= get_right() && m_ypos <= b.m_ypos && b.get_bottom() <= get_bottom();
	}

	template<meta::is_arithmetic Type>
	inline bool Rect<Type>::overlaps(const Rect<Type>& b) noexcept
	{
		// Check for overlaps using math.
		const auto x = value_in_range(m_xpos, b.m_xpos, b.m_xpos + b.m_width) || value_in_range(b.m_xpos, m_xpos, m_xpos + m_width);
		const auto y = value_in_range(m_ypos, b.m_ypos, b.m_ypos + b.m_height) || value_in_range(b.m_ypos, m_ypos, m_ypos + m_height);

		return x && y;
	}

	template<meta::is_arithmetic Type>
	inline bool Rect<Type>::intersects(const Rect<Type>& b) const noexcept
	{
		return !(m_xpos >= b.get_right() || get_right() <= b.m_xpos || m_ypos >= b.get_bottom() || get_bottom() <= b.m_ypos);
	}

	template<meta::is_arithmetic Type>
	inline void Rect<Type>::set_top_left(const glm::vec<2, Type, glm::defaultp>& pos) noexcept
	{
		m_xpos = pos.x;
		m_ypos = pos.y;
	}

	template<meta::is_arithmetic Type>
	inline Type Rect<Type>::get_right() const noexcept
	{
		return m_xpos + m_width;
	}

	template<meta::is_arithmetic Type>
	inline Type Rect<Type>::get_bottom() const noexcept
	{
		return m_ypos + m_height;
	}

	template<meta::is_arithmetic Type>
	inline glm::vec<2, Type, glm::defaultp> Rect<Type>::get_top_left() const noexcept
	{
		return {m_xpos, m_ypos};
	}

	template<meta::is_arithmetic Type>
	inline glm::vec<2, Type, glm::defaultp> Rect<Type>::get_center() const noexcept
	{
		return {(m_xpos + m_width) / 2.0, (m_ypos + m_height) / 2.0};
	}

	template<meta::is_arithmetic Type>
	inline glm::vec<2, Type, glm::defaultp> Rect<Type>::get_size() const noexcept
	{
		return {m_width, m_height};
	}

	template<meta::is_arithmetic Type>
	inline bool Rect<Type>::value_in_range(const Type value, const Type min, const Type max) noexcept
	{
		// Check if a value is between min and max - i.e. in range.
		return (value >= min) && (value <= max);
	}
} // namespace galaxy

#endif
