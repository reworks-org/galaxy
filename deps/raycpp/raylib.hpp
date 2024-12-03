/**
 * C++ header to wrap raymath.h.
 */
#ifndef RAYLIB_CPP_INCLUDE_RAYMATH_HPP_
#define RAYLIB_CPP_INCLUDE_RAYMATH_HPP_

#ifdef __cplusplus
extern "C"
{
#include <raylib.h>
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
#endif

#include <raylib-media/raymedia.h>

namespace ray
{
	using ConfigFlags             = ::ConfigFlags;
	using TraceLogLevel           = ::TraceLogLevel;
	using KeyboardKey             = ::KeyboardKey;
	using MouseButton             = ::MouseButton;
	using MouseCursor             = ::MouseCursor;
	using GamepadButton           = ::GamepadButton;
	using GamepadAxis             = ::GamepadAxis;
	using MaterialMapIndex        = ::MaterialMapIndex;
	using ShaderLocationIndex     = ::ShaderLocationIndex;
	using ShaderUniformDataType   = ::ShaderUniformDataType;
	using ShaderAttributeDataType = ::ShaderAttributeDataType;
	using PixelFormat             = ::PixelFormat;
	using TextureFilter           = ::TextureFilter;
	using TextureWrap             = ::TextureWrap;
	using CubemapLayout           = ::CubemapLayout;
	using FontType                = ::FontType;
	using BlendMode               = ::BlendMode;
	using Gesture                 = ::Gesture;
	using CameraMode              = ::CameraMode;
	using CameraProjection        = ::CameraProjection;
	using NPatchLayout            = ::NPatchLayout;
	using MediaLoadFlag           = ::MediaLoadFlag;
	using MediaState              = ::MediaState;
	using MediaConfigFlag         = ::MediaConfigFlag;
	using MediaAudioFormat        = ::MediaAudioFormat;
	using MediaProperties         = ::MediaProperties;
} // namespace ray

#endif
