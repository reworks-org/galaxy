/**
 * C++ header to wrap raymath.h.
 */
#ifndef RAYLIB_CPP_INCLUDE_RAYMATH_HPP_
#define RAYLIB_CPP_INCLUDE_RAYMATH_HPP_

#ifdef __cplusplus
namespace rl
{
	extern "C"
	{
#include <raylib-media/raymedia.h>
#include <rlgl.h>
#endif
#ifndef RAYLIB_CPP_NO_MATH
#ifndef RAYMATH_STATIC_INLINE
#define RAYMATH_STATIC_INLINE
#endif
#define RAYMATH_DISABLE_CPP_OPERATORS
#include <raymath.h>
#endif
#ifdef __cplusplus
	}
}
#endif

#endif
