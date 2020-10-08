///
/// QSAPI.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_QSAPI_HPP_
#define QUASAR_QSAPI_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Use OpenGL API when compiling quasar.
	///
	static inline constexpr int API_GL = 0;

	///
	/// Use Vulkan API when compiling quasar.
	///
	static inline constexpr int API_VULKAN = 1;

	///
	/// Use DirectX API when compiling quasar.
	///
	static inline constexpr int API_DIRECTX = 2;

} // namespace qs

#endif