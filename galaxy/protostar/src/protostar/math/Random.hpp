///
/// Random.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RANDOM_HPP_
#define PROTOSTAR_RANDOM_HPP_

#include "protostar/system/Concepts.hpp"

#include <random>

///
/// Core namespace.
///
namespace pr
{
	///
	/// Credits: http://stackoverflow.com/a/32907541.
	///
	template<typename T>
	using conditional_distribution = std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::conditional_t<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, void>>;

	///
	/// Generate a random number of type T.
	///
	/// \param min Minimum number inclusive.
	/// \param max Maximum number inclusive.
	///
	/// \return Returns number of the same type as inputs.
	///
	template<IsArithmetic Type>
	[[nodiscard]] inline Type random(const Type min, const Type max) noexcept
	{
		// Reseed.
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// Restricts to one thread, so each thread gets a random device.
		thread_local static std::random_device rd;
		thread_local static std::mt19937 mt(rd());

		thread_local conditional_distribution<Type> dist(min, max);

		return dist(mt);
	}
}

#endif