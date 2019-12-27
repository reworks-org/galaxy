///
/// Vector2.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_VECTOR2_HPP_
#define PROTOSTAR_VECTOR2_HPP_

#include <type_traits>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// Represents a 2 variable vector object.
	///
	template<typename Type>
	class Vector2 final
	{
		///
		/// Make sure is integral or floating.
		///
		static_assert(std::is_arithmetic<Type>::value);

	public:
		///
		/// Default constructor.
		///
		Vector2();
		
		///
		/// Argument constructor.
		///
		/// \param x X value.
		/// \param y Y value.
		///
		Vector2(const Type x, const Type y);

		///
		/// Default destructor.
		///
		~Vector2() noexcept = default;

		///
		/// Swaps x and y.
		///
		void transpose();

	public:
		///
		/// X value.
		///
		Type m_x;

		///
		/// Y value.
		///
		Type m_y;
	};

	template<typename Type>
	inline Vector2<Type>::Vector2()
		:m_x(0), m_y(0)
	{
	}

	template<typename Type>
	inline Vector2<Type>::Vector2(const Type x, const Type y)
		:m_x(x), m_y(y)
	{
	}

	template<typename Type>
	inline void Vector2<Type>::transpose()
	{
		// Swaps X and Y.
		Type oldX = m_x;

		m_x = m_y;
		m_y = oldX;
	}
	
	///
	/// Negative operator overload.
	///
	template<typename Type>
	inline Vector2<Type> operator-(const Vector2<Type>& a)
	{
		return Vector2<Type>(-a.m_x, -a.m_y);
	}

	///
	/// Addition assignment operator overload.
	///
	template<typename Type>
	inline Vector2<Type>& operator+=(Vector2<Type>& a, const Vector2<Type>& b)
	{
		a.m_x += b.m_x;
		a.m_y += b.m_y;

		return a;
	}

	///
	/// Subtraction assignment operator overload.
	///
	template<typename Type>
	inline Vector2<Type>& operator-=(Vector2<Type>& a, const Vector2<Type>& b)
	{
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;

		return a;
	}

	///
	/// Addition operator overload.
	///
	template<typename Type>
	inline Vector2<Type> operator+(const Vector2<Type>& a, const Vector2<Type>& b)
	{
		return Vector2<Type>(a.m_x + b.m_x, a.m_y + b.m_y);
	}

	///
	/// Subtraction operator overload.
	///
	template<typename Type>
	inline Vector2<Type> operator-(const Vector2<Type>& a, const Vector2<Type>& b)
	{
	
		return Vector2<Type>(a.m_x - b.m_x, a.m_y - b.m_y);
	}

	///
	/// Equality operator overload.
	///
	template<typename Type>
	inline bool operator==(const Vector2<Type>& a, const Vector2<Type>& b)
	{
		return (a.m_x == b.m_x) && (a.m_y == b.m_y);
	}

	///
	/// Not operator overload.
	///
	template<typename Type>
	inline bool operator!=(const Vector2<Type>& a, const Vector2<Type>& b)
	{
		return (a.m_x != b.m_x) || (a.m_y != b.m_y);
	}
}

#endif