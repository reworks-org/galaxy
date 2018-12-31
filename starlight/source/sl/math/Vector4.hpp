///
/// Vector4.hpp
/// starlight
///
/// Created by reworks on 28/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_VECTOR4_HPP_
#define STARLIGHT_VECTOR4_HPP_

#include "sl/libs/cereal/access.hpp"

namespace sl
{
	///
	/// Represents a Vector4 object.
	///
	template<typename T>
	class Vector4 final
	{
		friend class cereal::access;
	public:
		///
		/// Default constructor. Values are initialized to 0.
		///
		Vector4();

		///
		/// Value constructor.
		///
		/// \param w W value.
		/// \param x X value.
		/// \param y Y value.
		/// \param z Z value.
		///
		Vector4(const T w, const T x, const T y, const T z);

	private:
		///
		/// Cereal serialize function.
		///
		template <class Archive>
		inline void serialize(Archive& ar)
		{
			ar(m_w, m_x, m_y, m_z);
		}

	public:
		///
		/// w value.
		///
		T m_w;

		///
		/// x value.
		///
		T m_x;

		///
		/// y value.
		///
		T m_y;

		///
		/// z value.
		///
		T m_z;
	};

	template<typename T>
	inline Vector4<T>::Vector4()
		:m_w(0), m_x(0), m_y(0), m_z(0)
	{
		// Default constructor.
	}

	template<typename T>
	inline Vector4<T>::Vector4(const T w, const T x, const T y, const T z)
		:m_w(w), m_x(x), m_y(y), m_z(z)
	{
		// Argument constructor.
	}

	///
	/// Operator overloading for Vector4 negative.
	///
	template<typename T>
	inline Vector4<T> operator-(const Vector4<T>& a)
	{
		return Vector4<T>(-a.m_w, -a.m_x, -a.m_y, -a.m_z);
	}

	///
	/// Operator overloading for Vector4 addition assignment.
	///
	template<typename T>
	inline Vector4<T>& operator+=(Vector4<T>& a, const Vector4<T>& b)
	{
		a.m_w += b.m_w;
		a.m_x += b.m_x;
		a.m_y += b.m_y;
		a.m_z += b.m_z;

		return a;
	}

	///
	/// Operator overloading for Vector4 subtraction assignment.
	///
	template<typename T>
	inline Vector4<T>& operator-=(Vector4<T>& a, const Vector4<T>& b)
	{
		a.m_w -= b.m_w;
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;
		a.m_z -= b.m_z;

		return a;
	}

	///
	/// Operator overloading for Vector4 addition.
	///
	template<typename T>
	inline Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b)
	{
		return Vector4<T>(a.m_w + b.m_w, a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	///
	/// Operator overloading for Vector4 subtraction.
	///
	template<typename T>
	inline Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b)
	{
		return Vector4<T>(a.m_w - b.m_w, a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
	}

	///
	/// Operator overloading for Vector4 equality
	///
	template<typename T>
	inline bool operator==(const Vector4<T>& a, const Vector4<T>& b)
	{
		return (a.m_w == b.m_w) && (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_z == b.m_z);
	}

	///
	/// Operator overloading for Vector4 not equal operator.
	///
	template<typename T>
	inline bool operator!=(const Vector4<T>& a, const Vector4<T>& b)
	{
		return (a.m_w != b.m_w) || (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_z != b.m_z);
	}
}

#endif