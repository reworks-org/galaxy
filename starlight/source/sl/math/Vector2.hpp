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
		/// \param x X value.
		/// \param y Y value.
		///
		Vector2(T x, T y);

		///
		/// Swaps x (width) and y (height).
		///
		void transpose();

	public:
		T m_x;
		T m_y;
	};

	template<typename T>
	Vector2<T>::Vector2()
		:m_x(0), m_y(0)
	{
	}

	template<typename T>
	Vector2<T>::Vector2(T x, T y)
		: m_x(x), m_y(y)
	{
	}

	template<typename T>
	inline void Vector2<T>::transpose()
	{
		T oldX = m_x;

		m_x = m_y;
		m_y = oldX;
	}
	
	template<typename T>
	Vector2<T> operator-(const Vector2<T>& a)
	{
		return Vector2<T>(-a.m_x, -a.m_y);
	}

	template<typename T>
	Vector2<T>& operator+=(Vector2<T>& a, const Vector2<T>& b)
	{
		a.m_x += b.m_x;
		a.m_y += b.m_y;

		return a;
	}

	template<typename T>
	Vector2<T>& operator-=(Vector2<T>& a, const Vector2<T>& b)
	{
		a.m_x -= b.m_x;
		a.m_y -= b.m_y;

		return a;
	}

	template<typename T>
	Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.m_x + b.m_x, a.m_y + b.m_y);
	}

	template<typename T>
	Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
	{
	
		return Vector2<T>(a.m_x - b.m_x, a.m_y - b.m_y);
	}

	template<typename T>
	bool operator==(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a.m_x == b.m_x) && (a.m_y == b.m_y);
	}

	template<typename T>
	bool operator!=(const Vector2<T>& a, const Vector2<T>& b)
	{
		return (a.m_x != b.m_x) || (a.m_y != b.m_y);
	}
}

#endif