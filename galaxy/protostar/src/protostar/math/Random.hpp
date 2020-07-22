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
	template<typename type>
	using conditional_distribution = std::conditional_t<
	    std::is_integral<type>::value,
	    std::uniform_int_distribution<type>,
	    std::conditional_t<std::is_floating_point<type>::value, std::uniform_real_distribution<type>, void>>;

	///
	/// Generate a random number of type T.
	///
	/// \param min Minimum number inclusive.
	/// \param max Maximum number inclusive.
	///
	/// \return Returns number of the same type as inputs.
	///
	template<is_arithmetic type>
	[[nodiscard]] inline type random(const type min, const type max)
	{
		// Reseed.
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// Restricts to one thread, so each thread gets a random device.
		thread_local std::random_device rd;
		thread_local std::mt19937 mt {rd()};

		thread_local conditional_distribution<type> dist {min, max};

		return dist(mt);
	}
} // namespace pr

#endif