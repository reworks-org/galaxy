///
/// Colours.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_COLOURS_HPP_
#define QUASAR_COLOURS_HPP_

#include <protostar/graphics/Colour.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Namespace holding predefined colours.
	///
	namespace Colours
	{
		///
		/// White.
		///
		static inline const protostar::Colour White { 255, 255, 255, protostar::Colour::OPAQUE };

		///
		/// Black.
		///
		static inline const protostar::Colour Black { 0, 0, 0, protostar::Colour::OPAQUE };

		///
		/// Transparent.
		///
		static inline const protostar::Colour Transparent { 0, 0, 0, protostar::Colour::TRANSPARENT };
	}
}

#endif