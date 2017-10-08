//
//  Vector3.hpp
//  rework
//
//  Created by reworks on 26/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_VECTOR3_HPP_
#define REWORK_VECTOR3_HPP_

namespace re
{
	template<typename T>
	class Vector3
	{
	public:
		///
		/// Default constructor. Values are initialized to 0.
		///
		Vector3();

		///
		/// Value constructor.
		///
		/// \param _x X value.
		/// \param _y Y value.
		/// \param _z Z value.
		///
		Vector3(T _x, T _y, T _z);

		///
		/// Negative overload.
		///
		Vector3<T> operator-(const Vector3<T>& a);

		///
		/// += overload.
		///
		Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b);

		///
		/// -= overload.
		///
		Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b);

		///
		/// + overload.
		///
		Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b);

		///
		/// - overload.
		///
		Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b);

		///
		/// == overload.
		///
		bool operator==(const Vector3<T>& a, const Vector3<T>& b);

		///
		/// != overload.
		///
		bool operator!=(const Vector3<T>& a, const Vector3<T>& b);

	public:
		T x;
		T y;
		T Z;
	};

	template<typename T>
	Vector3<T>::Vector3()
		:x(0), y(0), z(0)
	{
	}

	template<typename T>
	Vector3<T>::Vector3(T _x, T _y, T _z)
		: x(_x), y(_y), z(_z)
	{
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T>& a)
	{
		return Vector3<T>(-a.x, -a.y, -a.z);
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator+=(Vector3<T>& a, const Vector3<T>& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;

		return a;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator-=(Vector3<T>& a, const Vector3<T>& b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;

		return a;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T>& a, const Vector3<T>& b)
	{

		return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	template<typename T>
	bool Vector3<T>::operator==(const Vector3<T>& a, const Vector3<T>& b)
	{
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
	}

	template<typename T>
	bool Vector3<T>::operator!=(const Vector3<T>& a, const Vector3<T>& b)
	{
		return (a.x != b.x) || (a.y != b.y) || (a.z != b.z);
	}
}

#endif