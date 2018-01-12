///
///  Vector3.hpp
///  starlight
///
///  Created by reworks on 26/09/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_VECTOR3_HPP_
#define STARLIGHT_VECTOR3_HPP_

namespace sl
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
		/// \param x X value.
		/// \param y Y value.
		/// \param z Z value.
		///
		Vector3(T x, T y, T z);

		///
		/// Copy Constructor.
		///
		Vector3(const Vector3&) = default;

		///
		/// Move Constructor.
		///
		Vector3(Vector3&&) = default;

	public:
		T m_x;
		T m_y;
		T m_Z;
	};

	template<typename T>
	Vector3<T>::Vector3()
		:m_x(0), m_y(0), m_z(0)
	{
	}

	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z)
		: m_x(x), m_y(y), m_z(z)
	{
	}

	template<typename T>
	Vector3<T> operator-(const Vector3<T>& a)
	{
		return Vector3<T>(-a.m_x, -a.m_y, -a.m_z);
	}

	template<typename T>
	Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b)
	{
		a.m_x += b.m_x;
		a.m_y += b.m_y;
		a.m_z += b.m_z;

		return a;
	}

	template<typename T>
	Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b)
	{
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;
		a.m_z -= b.m_z;

		return a;
	}

	template<typename T>
	Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	template<typename T>
	Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b)
	{

		return Vector3<T>(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
	}

	template<typename T>
	bool operator==(const Vector3<T>& a, const Vector3<T>& b)
	{
		return (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_z == b.m_z);
	}

	template<typename T>
	bool operator!=(const Vector3<T>& a, const Vector3<T>& b)
	{
		return (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_z != b.m_z);
	}
}

#endif