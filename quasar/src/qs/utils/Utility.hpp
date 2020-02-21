///
/// Utility.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_UTILITY_HPP_
#define QUASAR_UTILITY_HPP_

#include <SDL2/SDL_stdinc.h>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Utility functions used by Quasar.
	///
	namespace Utils
	{
		///
		/// Converts a float to a unsigned 8 bit integer.
		///
		/// \param f Float to convert.
		///
		/// \return Uint8.
		///
		Uint8 floatToUint8(float f) noexcept;

		///
		/// Converts a from an unsigned 8 bit integer to a float.
		///
		/// \param f Float to convert.
		///
		/// \return Uint8.
		///
		float uint8ToFloat(Uint8 u8) noexcept;
	}
}

#endif