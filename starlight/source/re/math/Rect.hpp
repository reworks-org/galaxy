///
///  Rect.hpp
///  starlight
///
///  Created by reworks on 26/09/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_RECT_HPP_
#define REWORK_RECT_HPP_

namespace sl
{
	template<typename T1, typename T2 = T1>
	class Rect
	{
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
		/// \param w Width value.
		/// \param h Height value.
		///
		Rect(T1 x, T1 y, T2 width, T2 height);

		///
		/// Copy Constructor.
		///
		Rect(const Rect&) = default;

		///
		/// Move Constructor.
		///
		Rect(Rect&&) = default;

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

	public:
		T1 m_x;
		T1 m_y;
		T2 m_width;
		T2 m_height;

	private:
		///
		/// Private function to determine if value is in range.
		/// From: https://stackoverflow.com/a/306379
		///
		bool valueInRange(T1 value, T1 min, T1 max);
	};

	template<typename T1, typename T2 = T1>
	Rect<T1, T2>::Rect()
		:m_x(0), m_y(0), m_width(0), m_height(0)
	{
	}

	template<typename T1, typename T2 = T1>
	Rect<T1, T2>::Rect(T1 x, T1 y, T2 width, T2 height)
		: m_x(x), m_y(y), m_width(width), m_height(height)
	{
	}

	template<typename T1, typename T2 = T1>
	bool Rect<T1, T2>::contains(T1 x, T1 y)
	{
		bool out = ((x > m_x) && (x < (m_x + m_width)) && (y > m_y) && (y < (m_y + m_height))) ? true : false;

		return out;
	}

	template<typename T1, typename T2>
	bool Rect<T1, T2>::contains(const Rect<T1, T2>& b)
	{
		bool out = ((b.m_x + b.m_width) < (m_x + m_width) && (b.m_x) > (m_x) && (b.m_y) >(m_y) && (b.m_y + b.m_height) < (m_y + m_height)) ? true : false;

		return out;
	}

	template<typename T1, typename T2 = T1>
	bool Rect<T1, T2>::overlaps(const Rect<T1, T2>& b)
	{
		// Thanks: https://stackoverflow.com/a/306379

		bool xOverlap = valueInRange(m_x, b.m_x, b.m_x + b.m_width) ||
			valueInRange(b.m_x, m_x, m_x + m_width);

		bool yOverlap = valueInRange(m_y, b.m_y, b.m_y + b.m_height) ||
			valueInRange(b.m_y, m_y, m_y + m_height);

		return xOverlap && yOverlap;
	}

	template<typename T1, typename T2 = T1>
	bool Rect<T1, T2>::valueInRange(T1 value, T1 min, T1 max)
	{
		return (value >= min) && (value <= max);
	}

	template<typename T1, typename T2 = T1>
	Rect<T1, T2> operator-(const Rect<T1, T2>& a)
	{
		return Rect<T1, T2>(-a.m_x, -a.m_y, -a.m_width, -a.m_height);
	}

	template<typename T1, typename T2 = T1>
	Rect<T1, T2>& operator+=(Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		a.m_x += b.m_x;
		a.m_y += b.m_y;
		a.m_width += b.m_width;
		a.m_height += a.m_height;

		return a;
	}

	template<typename T1, typename T2 = T1>
	Rect<T1, T2>& operator-=(Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;
		a.m_width -= b.m_width;
		a.m_height -= b.m_height;

		return a;
	}

	template<typename T1, typename T2 = T1>
	Rect<T1, T2> operator+(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		return Rect<T1, T2>(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	template<typename T1, typename T2 = T1>
	Rect<T1, T2> operator-(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		return Rect<T1, T2>(a.m_x - b.m_x, a.m_y - b.m_y, a.m_width - b.m_width, a.m_height - b.m_height);
	}

	template<typename T1, typename T2 = T1>
	bool operator==(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		return (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_width == b.m_width) && (a.m_height == b.m_height);
	}

	template<typename T1, typename T2 = T1>
	bool operator!=(const Rect<T1, T2>& a, const Rect<T1, T2>& b)
	{
		return (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_width != b.m_width) || (a.m_height != b.m_height);
	}
}

#endif