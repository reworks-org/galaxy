///
/// Maths.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_MATHS_HPP_
#define PROTOSTAR_MATHS_HPP_

#include <type_traits>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// \brief Calculate the percentage in a custom range.
	///
	/// Credits: https://stackoverflow.com/a/25835683
	///
	/// \param input The value to check the percentage of.
	/// \param min The value representing 0%.
	/// \param max The value representing 100%.
	/// 
	/// \return Returns the percentage of the value in that range betwen 0 and 1. E.g. 39% is 0.39.
	///
	template<typename T>
	inline const T percentInRange(T input, T min, T max)
	{
		// Make sure is arimthmatic.
		static_assert(std::is_arithmetic<T>::value);

		return ((input - min) / (max - min));
	}
}

#endif