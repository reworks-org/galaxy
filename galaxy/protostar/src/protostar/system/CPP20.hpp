///
/// CPP20.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CPP20_HPP_
#define PROTOSTAR_CPP20_HPP_

#if !defined(__cpp_lib_jthread)
#	define STL_THREAD <expr/thread>
#else
#	define STL_THREAD <thread>
#endif

#if !defined(__cpp_lib_ranges)
#	define STL_RANGES <expr/ranges>
#else
#	define STL_RANGES <ranges>
#endif

#endif