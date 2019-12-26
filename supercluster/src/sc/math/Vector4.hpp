///
/// Vector4.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_VECTOR4_HPP_
#define SUPERCLUSTER_VECTOR4_HPP_

///
/// Core namespace.
///
namespace sc
{
	///
	/// Represents a Vector4 object.
	///
	template<typename Type>
	class Vector4 final
	{
	public:
		///
		/// Default constructor.
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
		Vector4(const Type w, const Type x, const Type y, const Type z);

	public:
		///
		/// W value.
		///
		Type m_w;

		///
		/// X value.
		///
		Type m_x;

		///
		/// Y value.
		///
		Type m_y;

		///
		/// Z value.
		///
		Type m_z;
	};

	template<typename Type>
	inline Vector4<Type>::Vector4()
		:m_w(0), m_x(0), m_y(0), m_z(0)
	{
	}

	template<typename Type>
	inline Vector4<Type>::Vector4(const Type w, const Type x, const Type y, const Type z)
		:m_w(w), m_x(x), m_y(y), m_z(z)
	{
	}

	///
	/// Operator overloading for Vector4 negative.
	///
	template<typename Type>
	inline Vector4<Type> operator-(const Vector4<Type>& a)
	{
		return Vector4<Type>(-a.m_w, -a.m_x, -a.m_y, -a.m_z);
	}

	///
	/// Operator overloading for Vector4 addition assignment.
	///
	template<typename Type>
	inline Vector4<Type>& operator+=(Vector4<Type>& a, const Vector4<Type>& b)
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
	template<typename Type>
	inline Vector4<Type>& operator-=(Vector4<Type>& a, const Vector4<Type>& b)
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
	template<typename Type>
	inline Vector4<Type> operator+(const Vector4<Type>& a, const Vector4<Type>& b)
	{
		return Vector4<Type>(a.m_w + b.m_w, a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	///
	/// Operator overloading for Vector4 subtraction.
	///
	template<typename Type>
	inline Vector4<Type> operator-(const Vector4<Type>& a, const Vector4<Type>& b)
	{
		return Vector4<Type>(a.m_w - b.m_w, a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
	}

	///
	/// Operator overloading for Vector4 equality
	///
	template<typename Type>
	inline bool operator==(const Vector4<Type>& a, const Vector4<Type>& b)
	{
		return (a.m_w == b.m_w) && (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_z == b.m_z);
	}

	///
	/// Operator overloading for Vector4 not equal operator.
	///
	template<typename Type>
	inline bool operator!=(const Vector4<Type>& a, const Vector4<Type>& b)
	{
		return (a.m_w != b.m_w) || (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_z != b.m_z);
	}
}

#endif