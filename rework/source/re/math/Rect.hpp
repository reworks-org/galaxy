//
//  Rect.hpp
//  REngine3
//
//  Created by reworks on 26/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_RECT_HPP_
#define RENGINE3_RECT_HPP_

namespace re
{
	template<typename T>
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
		/// \param _x X value.
		/// \param _y Y value.
		/// \param _w Width value.
		/// \param _h Height value.
		///
		Rect(T _x, T _y, T _width, T _height);

		///
		/// Does the rectangle contain the point (x, y).
		///
		/// \param x X coordinate.
		/// \param y Y coordinate.
		///
		/// \return true if contains the point.
		///
		bool contains(T _x, T _y);

		///
		/// Do the rectangles a and b overlap.
		///
		/// \param a Left rectangle.
		/// \param b Right rectangle.
		///
		/// \return Returns true if there is an overlap.
		///
		bool overlaps(const Rect<T>& a, const Rect<T>& b);

		///
		/// Negative overload.
		///
		Rect<T> operator-(const Rect<T>& a);

		///
		/// += overload.
		///
		Rect<T>& operator+=(Rect<T>& a, const Rect<T>& b);

		///
		/// -= overload.
		///
		Rect<T>& operator-=(Rect<T>& a, const Rect<T>& b);

		///
		/// + overload.
		///
		Rect<T> operator+(const Rect<T>& a, const Rect<T>& b);

		///
		/// - overload.
		///
		Rect<T> operator-(const Rect<T>& a, const Rect<T>& b);

		///
		/// == overload.
		///
		bool operator==(const Rect<T>& a, const Rect<T>& b);

		///
		/// != overload.
		///
		bool operator!=(const Rect<T>& a, const Rect<T>& b);

	public:
		T x;
		T y;
		T width;
		T height;

	private:
		///
		/// Private function to determine if value is in range.
		/// From: https://stackoverflow.com/a/306379
		///
		bool valueInRange(T value, T min, T max);
	};

	template<typename T>
	Rect<T>::Rect()
		:x(0), y(0), width(0), height(0)
	{
	}

	template<typename T>
	Rect<T>::Rect(T _x, T _y, T _width, T _height)
		: x(_x), y(_y), width(_width), height(_height)
	{
	}

	template<typename T>
	bool Rect<T>::contains(T _x, T _y)
	{
		bool out = false;

		if ((_x > x ) && (_x < (x + width)) && (_y > y) && (_y < (y + height)))
		{
			out = true;
		}

		return out;
	}

	template<typename T>
	bool Rect<T>::overlaps(const Rect<T>& a, const Rect<T>& b)
	{
		// Thanks: https://stackoverflow.com/a/306379

		bool xOverlap = valueInRange<T>(a.x, b.x, b.x + b.width) ||
			valueInRange<T>(b.x, a.x, a.x + a.width);

		bool yOverlap = valueInRange<T>(a.y, b.y, b.y + b.height) ||
			valueInRange<T>(b.y, a.y, a.y + a.height);

		return xOverlap && yOverlap;
	}

	template<typename T>
	Rect<T> Rect<T>::operator-(const Rect<T>& a)
	{
		return Rect<T>(-a.x, -a.y, -a.width, -a.height);
	}

	template<typename T>
	Rect<T>& Rect<T>::operator+=(Rect<T>& a, const Rect<T>& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.width += b.width;
		a.height += a.height;

		return a;
	}

	template<typename T>
	Rect<T>& Rect<T>::operator-=(Rect<T>& a, const Rect<T>& b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.width -= b.width;
		a.height -= b.height;

		return a;
	}

	template<typename T>
	Rect<T> Rect<T>::operator+(const Rect<T>& a, const Rect<T>& b)
	{
		return Rect<T>(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	template<typename T>
	Rect<T> Rect<T>::operator-(const Rect<T>& a, const Rect<T>& b)
	{
		return Rect<T>(a.x - b.x, a.y - b.y, a.width - b.width, a.height - b.height);
	}

	template<typename T>
	bool Rect<T>::operator==(const Rect<T>& a, const Rect<T>& b)
	{
		return (a.x == b.x) && (a.y == b.y) && (a.width == b.width) && (a.height == b.height);
	}

	template<typename T>
	bool Rect<T>::operator!=(const Rect<T>& a, const Rect<T>& b)
	{
		return (a.x != b.x) || (a.y != b.y) || (a.width != b.width) || (a.height != b.height);
	}

	template<typename T>
	bool Rect<T>::valueInRange(T value, T min, T max)
	{
		return (value >= min) && (value <= max);
	}
}

#endif