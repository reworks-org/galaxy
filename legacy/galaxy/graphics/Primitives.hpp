///
/// Primitives.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_PRIMITIVES_HPP_
#define GALAXY_GRAPHICS_PRIMITIVES_HPP_

#include <type_traits>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Primitive Types.
		///
		enum class Primitives : short
		{
			CIRCLE   = 0,
			ELLIPSE  = 1,
			LINE     = 2,
			POINT    = 3,
			POLYLINE = 4,
			POLYGON  = 5
		};
	} // namespace graphics
} // namespace galaxy

#endif