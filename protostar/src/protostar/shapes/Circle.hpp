///
/// Circle.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CIRCLE_HPP_
#define PROTOSTAR_CIRCLE_HPP_

#include <type_traits>

#include "protostar/math/Point.hpp"

///
/// Core namespace.
///
namespace protostar
{
	///
	/// Represents a circle shape.
	///
	template<typename Type>
	class Circle
	{
		///
		/// Make sure is integral or floating.
		///
		static_assert(std::is_arithmetic<Type>::value);

	public:
		///
		/// Default constructor.
		///
		Circle();

		///
		/// Default destructor.
		///
		~Circle() = default;

		///
		/// Point constructor.
		///
		/// \param radius 32bit unsigned integer defining circle radius.
		/// \param center Center point of circle from which to extend radius.
		///
		explicit Circle(const Type radius, const protostar::fPoint& center) noexcept;

		///
		/// X, Y constructor.
		///
		/// \param radius 32bit unsigned integer defining circle radius.
		/// \param centerX x-axis coord for center of circle.
		/// \param centerY y-axis coord for center of circle.
		///
		explicit Circle(const Type radius, const Type centerX, const Type centerY) noexcept;

		///
		/// Set radius of circle.
		///
		/// \param radius 32bit unsigned integer defining circle radius.
		///
		void setRadius(const Type radius);

		///
		/// Set center point of circle.
		///
		/// \param center Center point of circle from which to extend radius.
		///
		void setCenter(const protostar::fPoint& center);

		///
		/// Set center point of circle.
		///
		/// \param centerX x-axis coord for center of circle.
		/// \param centerY y-axis coord for center of circle.
		///
		void setCenter(const Type centerX, const Type centerY);

		///
		/// Get radius of circle.
		///
		/// \return 32bit unsigned integer circle radius.
		///
		const Type getRadius() const noexcept;
		
		///
		/// Get center point of circle.
		///
		/// \return Center point of circle.
		///
		const protostar::fPoint& getCenter() const noexcept;
		
	private:
		///
		/// Circle radius.
		///
		Type m_radius;

		///
		/// Circle center point.
		///
		protostar::fPoint m_center;
	};

	template<typename Type>
	inline Circle<Type>::Circle()
		:m_radius(0)
	{
	}

	template<typename Type>
	inline Circle<Type>::Circle(const Type radius, const protostar::fPoint& center) noexcept
		:m_radius(radius), m_center(center)
	{
	}

	template<typename Type>
	inline Circle<Type>::Circle(const Type radius, const Type centerX, const Type centerY) noexcept
		:m_radius(radius), m_center(centerX, centerY)
	{
	}

	template<typename Type>
	inline void Circle<Type>::setRadius(const Type radius)
	{
		m_radius = radius;
	}

	template<typename Type>
	inline void Circle<Type>::setCenter(const protostar::fPoint& center)
	{
		m_center = center;
	}

	template<typename Type>
	inline void Circle<Type>::setCenter(const Type centerX, const Type centerY)
	{
		m_center.m_x = centerX;
		m_center.m_y = centerY;
	}

	template<typename Type>
	inline const Type Circle<Type>::getRadius() const noexcept
	{
		return m_radius;
	}

	template<typename Type>
	inline const protostar::fPoint& Circle<Type>::getCenter() const noexcept
	{
		return m_center;
	}
}

#endif