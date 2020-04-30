///
/// Types.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_TYPES_HPP_
#define CELESTIAL_TYPES_HPP_

#include <string>
#include <utility>

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Shorthand for a font - size std::pair.
	///
	/// First param is the filepath, second is the font size.
	///
	using FontData = std::pair<std::string, int>;
}

#endif