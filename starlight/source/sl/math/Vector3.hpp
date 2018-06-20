///
/// Vector3.hpp
/// starlight
///
/// Created by reworks on 26/09/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_VECTOR3_HPP_
#define STARLIGHT_VECTOR3_HPP_

#include "sl/libs/cereal/access.hpp"

namespace sl
{
	///
	/// Represents a 3 variable vector object.
	///
	template<typename T>
	class Vector3 final
	{
		friend class cereal::access;
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

	private:
		///
		/// Cereal serialize function.
		///
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_x, m_y, m_z);
		}

	public:
		///
		/// x variable.
		///
		T m_x;

		///
		/// y variable.
		///
		T m_y;

		///
		/// z variable.
		///
		T m_z;
	};

	template<typename T>
	Vector3<T>::Vector3()
		:m_x(0), m_y(0), m_z(0)
	{
		// Default initialized constructor.
	}

	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z)
		: m_x(x), m_y(y), m_z(z)
	{
		// Argument constructor.
	}

	///
	/// Operator overloading for Vector3 negative.
	///
	template<typename T>
	Vector3<T> operator-(const Vector3<T>& a)
	{
		return Vector3<T>(-a.m_x, -a.m_y, -a.m_z);
	}

	///
	/// Operator overloading for Vector3 addition assignment.
	///
	template<typename T>
	Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b)
	{
		a.m_x += b.m_x;
		a.m_y += b.m_y;
		a.m_z += b.m_z;

		return a;
	}

	///
	/// Operator overloading for Vector3 subtraction assignment.
	///
	template<typename T>
	Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b)
	{
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;
		a.m_z -= b.m_z;

		return a;
	}

	///
	/// Operator overloading for Vector3 addition.
	///
	template<typename T>
	Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	///
	/// Operator overloading for Vector3 subtraction.
	///
	template<typename T>
	Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
	}

	///
	/// Operator overloading for Vector3 equality
	///
	template<typename T>
	bool operator==(const Vector3<T>& a, const Vector3<T>& b)
	{
		return (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_z == b.m_z);
	}

	///
	/// Operator overloading for Vector3 not equal operator.
	///
	template<typename T>
	bool operator!=(const Vector3<T>& a, const Vector3<T>& b)
	{
		return (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_z != b.m_z);
	}
}

#endif