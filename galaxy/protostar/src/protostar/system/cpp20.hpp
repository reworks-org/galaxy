///
/// cpp20.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CPP20_HPP_
#define PROTOSTAR_CPP20_HPP_

#include <version>

#if __has_include(<semaphore>)
#	define STL_SEMAPHORE <semaphore>
#else
#	define STL_SEMAPHORE <sync/semaphore>
#endif

#ifndef __cpp_lib_jthread
#	define STL_JTHREAD <jthread/jthread.hpp>
#else
#	define STL_JTHREAD assert("can be deleted");
#endif

#endif