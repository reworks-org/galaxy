///
/// cpp20.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CPP20_HPP_
#define PROTOSTAR_CPP20_HPP_

#include <version>

#ifndef __cpp_lib_semaphore
#define STL_SEMAPHORE <sync/semaphore>
#else
#define STL_SEMAPHORE <semaphore>
#endif

#ifndef __cpp_lib_jthread
#define STL_JTHREAD <jthread/jthread.hpp>
#else
#define STL_JTHREAD <thread>
#endif

#ifndef __cpp_lib_atomic_wait
#define STL_ATOMIC <sync/atomic_wait>
#else
#define STL_ATOMIC <atomic>
#endif

#ifndef __cpp_lib_format
#define STL_FORMAT <fmt/format.h>
#else
#define STL_FORMAT <format>
#endif

#if __cpp_lib_chrono != 201907L
#define STL_CHRONO #include <date/tz.h> \
                   #include <chrono>
#else
#define STL_CHRONO <chrono>
#endif

#ifndef __cpp_lib_ranges
#define STL_RANGES <ranges/v3/all.hpp>
#else
#define STL_RANGES <ranges>
#endif

#endif