///
/// Random.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RANDOM_HPP_
#define PROTOSTAR_RANDOM_HPP_

#include <ctime>
#include <random>

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Credits: http://stackoverflow.com/a/32907541.
	///
	template<typename Type>
	using conditional_distribution = std::conditional_t<
	    std::is_integral<Type>::value,
	    std::uniform_int_distribution<Type>,
	    std::conditional_t<std::is_floating_point<Type>::value, std::uniform_real_distribution<Type>, void>>;

	///
	/// Generate a random number of type T.
	///
	/// \param min Minimum number inclusive.
	/// \param max Maximum number inclusive.
	///
	/// \return Returns number of the same type as inputs.
	///
	template<is_arithmetic Type>
	[[nodiscard]] inline Type random(const Type min, const Type max)
	{
		// Restricts to one thread, so each thread gets a random device.
		thread_local static std::random_device rd;
		thread_local static std::mt19937_64 mt {rd()};

		mt.seed(static_cast<unsigned int>(std::time(nullptr)));
		thread_local conditional_distribution<Type> dist {min, max};

		return dist(mt);
	}
} // namespace pr

#endif