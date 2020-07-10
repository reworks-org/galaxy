///
/// Rect.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RECT_HPP_
#define PROTOSTAR_RECT_HPP_

#include <type_traits>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// Represents a rectangle object.
	///
	template<typename Type>
	class Rect final
	{
		///
		/// Make sure is integral or floating.
		///
		static_assert(std::is_arithmetic<Type>::value);

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
		Rect(const Type x, const Type y, const Type width, const Type height) noexcept;

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
		bool contains(const Type x, const Type y) noexcept;

		///
		/// Does the rectangle contain another rectangle.
		///
		/// \param b Rectangle to check.
		///
		/// \return Returns true if the rectangle is completely inside, not on the edge.
		///
		bool contains(const Rect<Type>& b) noexcept;

		///
		/// \brief Do the rectangles a and b overlap.
		///
		/// Credits: https://stackoverflow.com/a/306379
		///
		/// \param b Second rectangle to test.
		///
		/// \return Returns true if there is an overlap.
		///
		bool overlaps(const Rect<Type>& b) noexcept;

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
		bool valueInRange(const Type value, const Type min, const Type max) noexcept;

	public:
		///
		/// X position.
		///
		Type m_x;

		///
		/// Y position.
		///
		Type m_y;

		///
		/// Width of rectangle.
		///
		Type m_width;

		///
		/// Height of rectangle.
		///
		Type m_height;
	};

	template<typename Type>
	inline Rect<Type>::Rect() noexcept
		:m_x(0), m_y(0), m_width(0), m_height(0)
	{
	}

	template<typename Type>
	inline Rect<Type>::Rect(const Type x, const Type y, const Type width, const Type height) noexcept
		:m_x(x), m_y(y), m_width(width), m_height(height)
	{
	}

	template<typename Type>
	inline bool Rect<Type>::contains(const Type x, const Type y) noexcept
	{
		// Checks if the rectangle contains the point (x, y) using some basic math.
		return ((x > m_x) && (x < (m_x + m_width)) && (y > m_y) && (y < (m_y + m_height)));
	}

	template<typename Type>
	inline bool Rect<Type>::contains(const Rect<Type>& b) noexcept
	{
		// Checks if the rectangle contains another rectangle using math.
		return ((b.m_x + b.m_width) < (m_x + m_width) && (b.m_x) > (m_x) && (b.m_y) > (m_y) && (b.m_y + b.m_height) < (m_y + m_height));
	}

	template<typename Type>
	inline bool Rect<Type>::overlaps(const Rect<Type>& b) noexcept
	{
		// Check for overlaps using math.
		bool xOverlap = valueInRange(m_x, b.m_x, b.m_x + b.m_width) ||
			valueInRange(b.m_x, m_x, m_x + m_width);

		bool yOverlap = valueInRange(m_y, b.m_y, b.m_y + b.m_height) ||
			valueInRange(b.m_y, m_y, m_y + m_height);

		return xOverlap && yOverlap;
	}

	template<typename Type>
	inline bool Rect<Type>::valueInRange(const Type value, const Type min, const Type max) noexcept
	{
		// Check if a value is between min and max - i.e. in range.
		return (value >= min) && (value <= max);
	}

	///
	/// Negative operator overload.
	///
	template<typename Type>
	inline Rect<Type> operator-(const Rect<Type>& a) noexcept
	{
		// Negative operator overloading.
		return Rect<Type>(-a.m_x, -a.m_y, -a.m_width, -a.m_height);
	}

	///
	/// Addition assignment operator overload.
	///
	template<typename Type>
	inline Rect<Type>& operator+=(Rect<Type>& a, const Rect<Type>& b) noexcept
	{
		// Addition assignment operator overloading.
		a.m_x += b.m_x;
		a.m_y += b.m_y;
		a.m_width += b.m_width;
		a.m_height += a.m_height;

		return a;
	}

	///
	/// Subtraction assignment operator overload.
	///
	template<typename Type>
	inline Rect<Type>& operator-=(Rect<Type>& a, const Rect<Type>& b) noexcept
	{
		// Subtraction assignment operator overloading.
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;
		a.m_width -= b.m_width;
		a.m_height -= b.m_height;

		return a;
	}

	///
	/// Addition operator overload.
	///
	template<typename Type>
	inline Rect<Type> operator+(const Rect<Type>& a, const Rect<Type>& b) noexcept
	{
		return Rect<Type>(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	///
	/// Subtraction operator overload.
	///
	template<typename Type>
	inline Rect<Type> operator-(const Rect<Type>& a, const Rect<Type>& b) noexcept
	{
		// Subtraction operator overloading.
		return Rect<Type>(a.m_x - b.m_x, a.m_y - b.m_y, a.m_width - b.m_width, a.m_height - b.m_height);
	}

	///
	/// Equality operator overload.
	///
	template<typename Type>
	inline bool operator==(const Rect<Type>& a, const Rect<Type>& b) noexcept
	{
		// Equality operator overloading.
		return (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_width == b.m_width) && (a.m_height == b.m_height);
	}

	///
	/// Not operator overload.
	///
	template<typename Type>
	inline bool operator!=(const Rect<Type>& a, const Rect<Type>& b) noexcept
	{
		// Not equal to operator overloading.
		return (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_width != b.m_width) || (a.m_height != b.m_height);
	}
}

#endif