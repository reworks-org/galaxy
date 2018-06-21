///
/// Rect.hpp
/// starlight
///
/// Created by reworks on 26/09/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RECT_HPP_
#define STARLIGHT_RECT_HPP_

#include "sl/libs/cereal/access.hpp"

namespace sl
{
	///
	/// Represents a rectangle object.
	///
	template<typename T1, typename T2 = T1>
	class Rect final
	{
		friend class cereal::access;
	public:
		///
		/// Default constructor. Values are initialized to 0.
		///
		Rect();

		///
		/// Value constructor.
		///
		/// \param x X value.
		/// \param y Y value.
		/// \param width Width value.
		/// \param height Height value.
		///
		Rect(T1 x, T1 y, T2 width, T2 height);

		///
		/// Does the rectangle contain the point (x, y).
		///
		/// \param x X coordinate.
		/// \param y Y coordinate.
		///
		/// \return true if contains the point.
		///
		bool contains(T1 x, T1 y);

		///
		/// Does the rectangle contain another rectangle.
		///
		/// \param b Rectangle to check.
		///
		/// \return Returns true if the rectangle is completely inside, not on the edge.
		///
		bool contains(const Rect<T1, T2>& b);

		///
		/// Do the rectangles a and b overlap.
		///
		/// \param b Right rectangle.
		///
		/// \return Returns true if there is an overlap.
		///
		bool overlaps(const Rect<T1, T2>& b);

	private:
		///
		/// Private function to determine if value is in range.
		/// From: https://stackoverflow.com/a/306379
		///
		bool valueInRange(T1 value, T1 min, T1 max);

		///
		/// Cereal serialize function.
		///
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_x, m_y, m_width, m_height);
		}

	public:
		///
		/// x position.
		///
		T1 m_x;

		///
		/// y position.
		///
		T1 m_y;

		///
		/// width of rectangle.
		///
		T2 m_width;

		///
		/// height of rectangle.
		///
		T2 m_height;
	};

	template<typename T1, typename T2>
	Rect<T1, T2>::Rect()
		:m_x(0), m_y(0), m_width(0), m_height(0)
	{
		// Default constructor.
	}

	template<typename T1, typename T2>
	Rect<T1, T2>::Rect(T1 x, T1 y, T2 width, T2 height)
		: m_x(x), m_y(y), m_width(width), m_height(height)
	{
		// Argument constructor.
	}

	template<typename T1, typename T2>
	bool Rect<T1, T2>::contains(T1 x, T1 y)
	{
		// Checks if the rectangle contains the point (x, y) using some basic math.
		bool out = ((x > m_x) && (x < (m_x + m_width)) && (y > m_y) && (y < (m_y + m_height))) ? true : false;

		return out;
	}

	template<typename T1, typename T2>
	bool Rect<T1, T2>::contains(const Rect<T1, T2>& b)
	{
		// Checks if the rectangle contains another rectangle using math.
		bool out = ((b.m_x + b.m_width) < (m_x + m_width) && (b.m_x) > (m_x) && (b.m_y) >(m_y) && (b.m_y + b.m_height) < (m_y + m_height)) ? true : false;

		return out;
	}

	template<typename T1, typename T2>
	bool Rect<T1, T2>::overlaps(const Rect<T1, T2>& b)
	{
		// Thanks: https://stackoverflow.com/a/306379

		// Check for overlaps using math.
		bool xOverlap = valueInRange(m_x, b.m_x, b.m_x + b.m_width) ||
			valueInRange(b.m_x, m_x, m_x + m_width);

		bool yOverlap = valueInRange(m_y, b.m_y, b.m_y + b.m_height) ||
			valueInRange(b.m_y, m_y, m_y + m_height);

		return xOverlap && yOverlap;
	}

	template<typename T1, typename T2>
	bool Rect<T1, T2>::valueInRange(T1 value, T1 min, T1 max)
	{
		// Check if a value is between min and max - i.e. in range.
		return (value >= min) && (value <= max);
	}

	template<typename T1, typename T2>
	Rect<T1, T2> operator-(const Rect<T1, T2>& a)
	{
		// Negative operator overloading.
		return Rect<T1, T2>(-a.m_x, -a.m_y, -a.m_width, -a.m_height);
	}

	template<typename T1, typename T2>
	Rect<T1, T2>& operator+=(Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		// Addition assignment operator overloading.
		a.m_x += b.m_x;
		a.m_y += b.m_y;
		a.m_width += b.m_width;
		a.m_height += a.m_height;

		return a;
	}

	template<typename T1, typename T2>
	Rect<T1, T2>& operator-=(Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		// Subtraction assignment operator overloading.
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;
		a.m_width -= b.m_width;
		a.m_height -= b.m_height;

		return a;
	}

	template<typename T1, typename T2>
	Rect<T1, T2> operator+(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		return Rect<T1, T2>(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	template<typename T1, typename T2>
	Rect<T1, T2> operator-(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		// Subtraction operator overloading.
		return Rect<T1, T2>(a.m_x - b.m_x, a.m_y - b.m_y, a.m_width - b.m_width, a.m_height - b.m_height);
	}

	template<typename T1, typename T2>
	bool operator==(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		// Equality operator overloading.
		return (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_width == b.m_width) && (a.m_height == b.m_height);
	}

	template<typename T1, typename T2>
	bool operator!=(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		// Not equal to operator overloading.
		return (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_width != b.m_width) || (a.m_height != b.m_height);
	}
}

#endif