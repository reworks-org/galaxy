///
/// Vector3.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_VECTOR3_HPP_
#define SUPERCLUSTER_VECTOR3_HPP_

#include <type_traits>

///
/// Core namespace.
///
namespace sc
{
	///
	/// Represents a 3 variable vector object.
	///
	template<typename Type>
	class Vector3 final
	{
		///
		/// Make sure is integral or floating.
		///
		static_assert(std::is_arithmetic<Type>::value);

	public:
		///
		/// Default constructor.
		///
		Vector3();

		///
		/// Value constructor.
		///
		/// \param x X value.
		/// \param y Y value.
		/// \param z Z value.
		///
		Vector3(const Type x, const Type y, const Type z);

		///
		/// Default destructor.
		///
		~Vector3() noexcept = default;

	public:
		///
		/// X variable.
		///
		Type m_x;

		///
		/// Y variable.
		///
		Type m_y;

		///
		/// Z variable.
		///
		Type m_z;
	};

	template<typename Type>
	inline Vector3<Type>::Vector3()
		:m_x(0), m_y(0), m_z(0)
	{
	}

	template<typename Type>
	inline Vector3<Type>::Vector3(const Type x, const Type y, const Type z)
		:m_x(x), m_y(y), m_z(z)
	{
	}

	///
	/// Operator overloading for Vector3 negative.
	///
	template<typename Type>
	inline Vector3<Type> operator-(const Vector3<Type>& a)
	{
		return Vector3<Type>(-a.m_x, -a.m_y, -a.m_z);
	}

	///
	/// Operator overloading for Vector3 addition assignment.
	///
	template<typename Type>
	inline Vector3<Type>& operator+=(Vector3<Type>& a, const Vector3<Type>& b)
	{
		a.m_x += b.m_x;
		a.m_y += b.m_y;
		a.m_z += b.m_z;

		return a;
	}

	///
	/// Operator overloading for Vector3 subtraction assignment.
	///
	template<typename Type>
	inline Vector3<Type>& operator-=(Vector3<Type>& a, const Vector3<Type>& b)
	{
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;
		a.m_z -= b.m_z;

		return a;
	}

	///
	/// Operator overloading for Vector3 addition.
	///
	template<typename Type>
	inline Vector3<Type> operator+(const Vector3<Type>& a, const Vector3<Type>& b)
	{
		return Vector3<Type>(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
	}

	///
	/// Operator overloading for Vector3 subtraction.
	///
	template<typename Type>
	inline Vector3<Type> operator-(const Vector3<Type>& a, const Vector3<Type>& b)
	{
		return Vector3<Type>(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
	}

	///
	/// Operator overloading for Vector3 equality
	///
	template<typename Type>
	inline bool operator==(const Vector3<Type>& a, const Vector3<Type>& b)
	{
		return (a.m_x == b.m_x) && (a.m_y == b.m_y) && (a.m_z == b.m_z);
	}

	///
	/// Operator overloading for Vector3 not equal operator.
	///
	template<typename Type>
	inline bool operator!=(const Vector3<Type>& a, const Vector3<Type>& b)
	{
		return (a.m_x != b.m_x) || (a.m_y != b.m_y) || (a.m_z != b.m_z);
	}
}

#endif