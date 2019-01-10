///
/// Vector2.hpp
/// starlight
///
/// Created by reworks on 26/09/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_VECTOR2_HPP_
#define STARLIGHT_VECTOR2_HPP_

namespace sl
{
	///
	/// Represents a 2 variable vector object.
	///
	template<typename T>
	class Vector2 final
	{
	public:
		///
		/// Default constructor. Values are initialized to 0.
		///
		Vector2();
		
		///
		/// Value constructor.
		///
		/// \param x X value.
		/// \param y Y value.
		///
		Vector2(const T x, const T y);

		///
		/// Swaps x and y.
		///
		void transpose();

	public:
		///
		/// X value.
		///
		T m_x;

		///
		/// Y value.
		///
		T m_y;
	};

	template<typename T>
	inline Vector2<T>::Vector2()
		:m_x(0), m_y(0)
	{
		// Default initialized constructor.
	}

	template<typename T>
	inline Vector2<T>::Vector2(const T x, const T y)
		: m_x(x), m_y(y)
	{
		// Argument constructor.
	}

	template<typename T>
	inline void Vector2<T>::transpose()
	{
		// Swaps X and Y.
		T oldX = m_x;

		m_x = m_y;
		m_y = oldX;
	}
	
	///
	/// Negative operator overload.
	///
	template<typename T>
	inline Vector2<T> operator-(const Vector2<T>& a)
	{
		return Vector2<T>(-a.m_x, -a.m_y);
	}

	///
	/// Addition assignment operator overload.
	///
	template<typename T>
	inline Vector2<T>& operator+=(Vector2<T>& a, const Vector2<T>& b)
	{
		a.m_x += b.m_x;
		a.m_y += b.m_y;

		return a;
	}

	///
	/// Subtraction assignment operator overload.
	///
	template<typename T>
	inline Vector2<T>& operator-=(Vector2<T>& a, const Vector2<T>& b)
	{
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;

		return a;
	}

	///
	/// Addition operator overload.
	///
	template<typename T>
	inline Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.m_x + b.m_x, a.m_y + b.m_y);
	}

	///
	/// Subtraction operator overload.
	///
	template<typename T>
	inline Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
	{
	
		return Vector2<T>(a.m_x - b.m_x, a.m_y - b.m_y);
	}

	///
	/// Equality operator overload.
	///
	template<typename T>
	inline bool operator==(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a.m_x == b.m_x) && (a.m_y == b.m_y);
	}

	///
	/// Not operator overload.
	///
	template<typename T>
	inline bool operator!=(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a.m_x != b.m_x) || (a.m_y != b.m_y);
	}
}

#endif