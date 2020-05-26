///
/// Utility.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_UTILITY_HPP_
#define QUASAR_UTILITY_HPP_

#include <cstdint>
#include <type_traits>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Utility functions used by Quasar.
	///
	namespace Utils
	{
		///
		/// Converts a float to a unsigned 8 bit integer.
		///
		/// \param f Float to convert.
		///
		/// \return Uint8.
		///
		std::uint8_t floatToUint8(float f) noexcept;

		///
		/// Converts a from an unsigned 8 bit integer to a float.
		///
		/// \param f Float to convert.
		///
		/// \return Uint8.
		///
		float uint8ToFloat(std::uint8_t u8) noexcept;

		///
		/// Calc normalized value from range.
		///
		/// \param val Value to divide by max.
		/// \param max Value representing 1.0f.
		///
		/// \return Normalized float.
		///
		template<typename T>
		inline float toNorm(const T val, const T max) noexcept
		{
			static_assert(std::is_arithmetic<T>::value);
			return (static_cast<float>(val) / static_cast<float>(max));
		}
	}
}

#endif