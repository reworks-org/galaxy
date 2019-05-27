///
/// Config.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_CONFIG_HPP_
#define SOLAR_CONFIG_HPP_

#include <cstdint>

#if defined(_WIN64) || defined(__x86_64__)
	#define SIXTYFOURBIT
#else
	#define THIRTYTWOBIT
#endif

#ifdef SIXTYFOURBIT
	#define SR_INTEGER std::uint64_t
#else
	#define SR_INTEGER std::uint32_t
#endif

namespace sr
{
	using Entity = SR_INTEGER;
}

#endif