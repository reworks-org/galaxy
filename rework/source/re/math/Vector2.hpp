//
//  Vector2.hpp
//  rework
//
//  Created by reworks on 26/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_VECTOR2_HPP_
#define REWORK_VECTOR2_HPP_

namespace re
{
	template<typename T>
	class Vector2
	{
	public:
		///
		/// Default constructor. Values are initialized to 0.
		///
		Vector2();
		
		///
		/// Value constructor.
		///
		/// \param _x X value.
		/// \param _y Y value.
		///
		Vector2(T _x, T _y);

		///
		/// For when it makes sense to use width over x. Kinda like std::string's length() and size().
		///
		T width();

		///
		/// For when it makes sense to use height over y. Kinda like std::string's length() and size().
		///
		T height();

	public:
		T x;
		T y;
	};

	template<typename T>
	Vector2<T>::Vector2()
		:x(0), y(0)
	{
	}

	template<typename T>
	Vector2<T>::Vector2(T _x, T _y)
		:x(_x), y(_y)
	{
	}

	template<typename T>
	inline T Vector2<T>::width()
	{
		return x;
	}

	template<typename T>
	inline T Vector2<T>::height()
	{
		return y;
	}
	
	template<typename T>
	Vector2<T> operator-(const Vector2<T>& a)
	{
		return Vector2<T>(-a.x, -a.y);
	}

	template<typename T>
	Vector2<T>& operator+=(Vector2<T>& a, const Vector2<T>& b)
	{
		a.x += b.x;
		a.y += b.y;

		return a;
	}

	template<typename T>
	Vector2<T>& operator-=(Vector2<T>& a, const Vector2<T>& b)
	{
		a.x -= b.x;
		a.y -= b.y;

		return a;
	}

	template<typename T>
	Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x + b.x, a.y + b.y);
	}

	template<typename T>
	Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
	{
	
		return Vector2<T>(a.x - b.x, a.y - b.y);
	}

	template<typename T>
	bool operator==(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a.x == b.x) && (a.y == b.y);
	}

	template<typename T>
	bool operator!=(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a.x != b.x) || (a.y != b.y);
	}
}

#endif