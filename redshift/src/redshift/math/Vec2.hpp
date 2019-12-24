///
/// Vec2.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_VEC2_HPP_
#define REDSHIFT_VEC2_HPP_

#include <stdexcept>
#include <type_traits>

///
/// Core namespace.
///
namespace rs
{
	///
	/// Basic Vec2 data structure.
	///
	template<typename Type>
	struct Vec2
	{
		// Ensure structure is arithmetic (integral/float).
		static_assert(std::is_arithmetic<Type>::value);

		///
		/// Default constructor.
		///
		Vec2() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x First value.
		/// \param y Second value.
		///
		Vec2(const Type x, const Type y) noexcept;

		///
		/// Default destructor.
		///
		~Vec2() = default;

		///
		/// X value in a vector.
		///
		Type m_x;

		///
		/// Y value in a vector.
		///
		Type m_y;
	};

	template<typename Type>
	inline Vec2<Type>::Vec2() noexcept
		:m_x(0), m_y(0)
	{
	}

	template<typename Type>
	inline Vec2<Type>::Vec2(const Type x, const Type y) noexcept
		:m_x(x), m_y(y)
	{
	}
}

#endif